# django-Haystack

django 的第三方插件，提供可拔插式的搜索后端。支持 [Solr](http://lucene.apache.org/solr/)， [Elasticsearch](http://elasticsearch.org/)，[Whoosh](https://bitbucket.org/mchaput/whoosh/)，[Xapian](http://xapian.org/)

等搜索引擎。是项目于搜索引擎解耦和。

<!-- more -->

### 1.安装

------

> `pip install django-haystack`

**1.安装引擎 （以 whoosh 为例）**

`pip install whoosh`

**2.中文搜索，**

因 whoosh 默认的分词是英文的对中文支持不是很友好，所以需要从新配置使用更加友好的 jieba 分词。

可直接修改 whoosh 的源码实现，当然这样是不明智的！

把 haystack\backends\whoosh_backend.py 文件拷贝到 app 目录下，并重命名为 whoosh_cn_backend.py 

做一下修改：

```python
+ from jieba.analyse import ChineseAnalyzer

- schema_fields[field_class.index_fieldname] = TEXT(stored=True, analyzer=StemmingAnalyzer(), field_boost=field_class.boost, sortable=True)
+ schema_fields[field_class.index_fieldname] = TEXT(stored=True, analyzer=ChineseAnalyzer(),field_boost=field_class.boost, sortable=True)
```



### 2.配置django project

------

**配置 settings.py **

```python
INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'patients.apps.PatientsConfig',
    'haystack', # 添加 app
]

# 配置 haystack
HAYSTACK_CONNECTIONS = {
    'default': {
        'ENGINE': 'patients.whoosh_cn_backend.WhooshEngine',  # 选择引擎（修改后支持中文引擎, patients 为 app 名）
        'PATH': os.path.join(BASE_DIR, 'whoosh_index'),  # 索引存放的位置，需要手动创建
        'INCLUDE_SPELLING': True
    },
}

# 设置每页显示的数据量
HAYSTACK_SEARCH_RESULTS_PER_PAGE = 50
# 当数据库改变时，会自动更新索引
HAYSTACK_SIGNAL_PROCESSOR = 'haystack.signals.RealtimeSignalProcessor'

# 其他详细配置参见官方文档
```

\# `HAYSTACK_SIGNAL_PROCESSOR`是通过 django 的 save 信号来更新的。直接操作数据库是不会更新索引的。

### 3.原理及各文件编写（whoosh）

------

#### 1.为数据的特定字段创建索引，用于快速搜索

##### 一、创建索引类

*索引类本质就是做一个到 model 的映射关系，可以设置检索字段、过滤数据 等其他自定设置。*

**1.** 在 app 下创建 search_indexes.py 。默认文件名，请勿修改

**2.**编写 search_indexes.py 如下：

```python
from haystack import indexes
from .models import Main

class MainIndex(indexes.SearchIndex, indexes.Indexable):  # 类名必须为模型名+Index
    text = indexes.CharField(document=True, use_template=True) 
    # use_template=True 使用索引模板建立索引文件
    
    def get_model(self):  # 设置模型
        return Main
    
    def index_queryset(self, using=None):  # 设置查询范围
        return self.get_model().objects.all()
```

**document=True **

> 1.使用此字段的内容作为索引进行检索。
>
> 2.每个索引类，都需要有一个字段设置为 True， 并且是唯一一个。
>
> 3.此字段名习惯命名为 text。

**use_template=True**

> 1.使用索引模板建立索引文件
>
> 2.索引模板的命名规则： 模型名（小写）+_text.txt。
>
> 3.保存路径：templates/search/indexes/myapp/  (一般放在 app 下)
>
> > myapp 是对应模型定义在的 app 名，不是当前app。
> >
> > \# 如在app1 里建立索引，但是用到模型是定义在 app2 中。那么保存的全路径应该是：
> >
> > 'myproject'/app1/templates/search/indexes/app2/'model_name'_text.txt
>
> 4.编写 patients\templates\search\indexes\patients\main_text.txt
>
> ```
> {{ object.drug }}
> {{ object.summary }}
> ```

\# 对应模型里的字段名

**text = indexes.CharField(document=True, use_template=True)**

> 根据 text 创建索引（document=True），text 对应对应数据库中的字段（在所有模板中编写）（use_template=True）



##### 二、通过命令创建索引

```python
python manage.py rebuild_index
```

\# 看到 `Prefix dict has been built succesfully.`  就表示创建成功了。

[管理命令说明 ](https://django-haystack.readthedocs.io/en/v2.8.1/management_commands.html)

1.会在根据 model 的 drug、summary 字段创建索引（索引类、索引模板设置）

2.索引保存在根目录下的 whoosh_index 文件夹下（settings 中设置）



#### 2.配置路由和编写视图函数

**1.配置路由 （urls.py）**

```python
	+ path('', MySearchView.as_view(), name='haystack')
```

**2.编写视图函数**

> 1.视图函数使用继承 haystack 封装好的 SearchView 类，并经行重载的方法实现。
>
> > 1.版本2.4开始不推荐使用 haystack.views.SearchView，而是使用更标准的 haystack.generic_views.SearchView
>
> 2.SearchView 封装了 表单（build_form）、查询（get_query、get_results）、返回 Response （render_to_response）等步骤。可通过重载实现自定义。
>
> 3.SearchView 默认使用的 Form 为 ModelSearchForm， ModelSearchForm 也是继承 SearchForm，即所有的 Form 类都因该继承 SearchForm（自定义）。

一、自定义表单

> 1.编写表单类， 并继承 SearchForm。
>
> 2.设置视图的 form_class 字段。
>
> 3.重载 search 方法。
>
> ```python
> from django import forms
> from haystack.forms import SearchForm
> 
> 
> class DateRangeSearchForm(SearchForm):
> # 在表单中定义 start_date， end_date 字段，默认只有一个 q 字段
> start_date = forms.DateField(required=False)
> end_date = forms.DateField(required=False)
> 
> def search(self):
> sqs = super(DateRangeSearchForm, self).search()  # 父类的结果
> 
> 
> if not self.is_valid():
>   return self.no_query_found()
> 
> if self.cleaned_data['start_date']:  # 如果表单中有 start_date，如何处理
>   sqs = sqs.filter(pub_date__gte=self.cleaned_data['start_date'])
> 
> if self.cleaned_data['end_date']:  # 如果表单中有 end_date，如何处理
>   sqs = sqs.filter(pub_date__lte=self.cleaned_data['end_date'])
> 
> return sqs
> ```
>
> 

二、自定义查询数据

> 重载 get_query 方法

三、自定义模板渲染过程

> 1.会先判断form是否有效，分别对应不同的函数form_invalid，form_valid，主要区别在于创建上下午数据。创建不同的 Response。
>
> 2.重载 get_context_data 实现对上下文数据的再处理。

> \# **得到的请求数据（`self.request.GET`）是 `<QueryDict: {'name': [''], 'weight': [''], 'size': ['']}>` 形式。即使是空的数据， is_valid 也是返回 True。**
>
> \# **这也就导致根本无法执行到 form_invalid 。**
>
> \# **暂时没有找到解决的办法，先绕行了，覆盖 get() 方法，以后再解决**

views.py

```python
class MySearchView(SearchView):
    template_name = 'search/search.html'  # 指定模板
    form_class = DateRangeSearchForm  # 自定自定义表单类

    def get_queryset(self):  # 查询集
        queryset = super(MySearchView, self).get_queryset()
        return queryset

    def get_context_data(self, *args, **kwargs):  # 上下文
        context = super(MySearchView, self).get_context_data(*args, **kwargs)
        return context

    def get(self, request, *args, **kwargs):  # get 请求

        form_class = self.get_form_class()
        form = self.get_form(form_class)
        self.queryset = form.search()

        cleaned_data = form.cleaned_data
        q = cleaned_data.get(self.search_field, None)  # 表单字段 q

        if q:  # 有，返回查找结果
            context = self.get_context_data(**{
                self.form_name: form,
                'query': form.cleaned_data.get(self.search_field),
                'object_list': self.queryset  # 查找结果
            })
            return render(self.request, template_name=self.template_name, context=locals())

        else: # 没有
            context = self.get_context_data(**{
                self.form_name: form,
                'query': form.cleaned_data.get(self.search_field),
                'object_list': self.get_queryset()
            })
            return render(self.request, template_name=self.template_name, context=locals())

```

[参考官方文档](https://django-haystack.readthedocs.io/en/v2.8.1/views_and_forms.html)



 **3.编写模板**

  `‘haystack ’`的上下文是自带**分页**功能的，是非常的方便的。默认上下文中包括如下字段：

> 'paginator': 分页对象
> 'page_obj': 当前页对象 
> 'is_paginated': 是否分页, 
> 'object_list': 当前页包含的所有数据。`SearchResult`类对象。[API](https://django-haystack.readthedocs.io/en/v2.8.1/searchresult_api.html)
> 'form': 表单对象, 
> 'query': 查询字段 
> 'view': 视图函数对象

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
    <!-- 生成表单 -->
    <form method="get" action="">  
        <table>
            {{ form.as_table }}
            <tr>
                <td>
                    <input type="submit" value="Search">
                </td>
            </tr>
        </table>
    </form>
	
    <!-- 展示数据 -->
    <div>
        {% if query %}  <!-- 判断是否有查询字段 -->
        
            {% for object in object_list %}  <!-- 遍历当前页的所有数据 -->
                <p>
                    <a >{{ object.object.drug }}</a>
                </p>
            {% empty %}  <!-- 结果为空时 -->
                <p>No results found.</p>
            {% endfor %}

            {% if is_paginated %}  <!-- 是否分页 -->
                {% if page_obj.has_previous %}  <!-- 是否有上一页 -->
                    <a href="{% url 'patients:haystack'%}?q={{ query }}&amp;page={{ page_obj.previous_page_number }}" class="prev">上一页</a>
                {% endif %}

                {% for page in paginator.page_range %} <!-- 列出所有页 -->
                    <a href="{% url 'patients:haystack' %}?q={{ query }}&amp;page={{ page }}" target="_self">{{page}}</a>
                {% endfor %}

                {% if page_obj.has_next %}  <!-- 是否有下一页 -->
                    <a href="{% url 'patients:haystack' %}?q={{ query }}&amp;page={{ page_obj.next_page_number }}" class="next">下一页</a>
                {% endif %}

            {% endif %}
        {% endif %}
    </div>

</body>
</html>

```



------

**问题1：在多个模型中查找**

  这个问题主要在于创建索引步骤。创建索引时可以创建多个索引，这样查找时就可以在多个模型中一起查找（默认），也可以指定在那个模型中查找，代码如下：

```python
from haystack import indexes
from .models import *

class MainIndex(indexes.SearchIndex, indexes.Indexable):  # 类名必须为模型名+Index
    text = indexes.CharField(document=True, use_template=True)

    def get_model(self):  # 设置模型
        return Main
    def index_queryset(self, using=None):  # 设置查询范围
        return self.get_model().objects.all()

class PatientBaseInfoIndex(indexes.SearchIndex, indexes.Indexable):
    text = indexes.CharField(document=True, use_template=True)

    def get_model(self):  
        return PatientBaseInfo
    def index_queryset(self, using=None):
        return self.get_model().objects.all()

```

\# 然后正常通过命令创建索引就可以。

\# 如果想在模板中判断查找的结果是那个模型的数据，可以定义模板过滤器的模型名。如下：

```python
@register.filter
def to_class_name(value):
    return value.__class__.__name__

```

```
{{ obj | to_class_name }} 

```



------

**问题2：多个搜索引擎**

直接在项目的 settings.py 中设置多个引擎就可以。

```python
HAYSTACK_CONNECTIONS = {
    'default': {
        'ENGINE': 'haystack.backends.solr_backend.SolrEngine',
        'URL': 'http://localhost:9001/solr/default',
        'TIMEOUT': 60 * 5,
        'INCLUDE_SPELLING': True,
        'BATCH_SIZE': 100,
        'SILENTLY_FAIL': True,
    },
    'autocomplete': {
        'ENGINE': 'haystack.backends.whoosh_backend.WhooshEngine',
        'PATH': '/home/search/whoosh_index',
        'STORAGE': 'file',
        'POST_LIMIT': 128 * 1024 * 1024,
        'INCLUDE_SPELLING': True,
        'BATCH_SIZE': 100,
        'SILENTLY_FAIL': True,
    },
    'slave': {
        'ENGINE': 'xapian_backend.XapianEngine',
        'PATH': '/home/search/xapian_index',
        'INCLUDE_SPELLING': True,
        'BATCH_SIZE': 100,
        'SILENTLY_FAIL': True,
    },
    'db': {
        'ENGINE': 'haystack.backends.simple_backend.SimpleEngine',
        'SILENTLY_FAIL': True,
    }
}

```

[官方说明](https://django-haystack.readthedocs.io/en/v2.8.1/multiple_index.html)



### SearchQuerySet

- Haystack 的查询方式主要是 SearchQuerySet 类实现。
- 查询方式类是与 django 的 Query。
- 主要有如下方法：

```python
from haystack.query import SearchQuerySet
Q = SearchQuerySet()

Q.filter(content='egfr')
Q.models(model_name).filter(content='egfr')

```

\# 其中查询参数有：

content、contains、exact、gt、gte、lt、lte、in、startswith、endswith、range、fuzzy

