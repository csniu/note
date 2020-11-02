# django-form

## 一、Form

  在 HTML 中， 表单是在 `<form>....</form>` 中的一些元素，用于收集不同类型的用户输入。

  一个完整的表单主要包括：

> \* 提交地址：action 属性指定，决定提交的数据由那个 url 接受和处理。若为空，则表示由当前 url 接受。
>
> \* 请求方式：method 属性指定。
>
> \* 元素控件：input 标签。获取输入数据，由其 type 属性定义输入数据的类型，常见如下：
>
> > ext：文本，默认是20个字符长
> >
> > password：密码，不会明文显示，而是以星号或圆点替代。
> >
> > radio：单选框
> >
> > checkbox：复选框
> >
> > submit：提交，当用户单击确认按钮时，表单的内容会被传送到另一个文件。表单的动作属性定义了目的文件的文件名。
>
> \* 提交按钮：input 标签，type=submit

```html
<form name="input" action="html_form_action.php" method="get">
    First name: <input type="text" name="firstname"><br>
    Last name: <input type="text" name="lastname">
    <input type="submit" value="Submit">
</form>
```

  [参考](http://www.runoob.com/html/html-forms.html)

<!-- more -->

### 二.django

 在模板中直接编写表单是很简单的事情，但是如果表单过多，会增加代码量，同时对日后的维护和更新带来很大的不便。django 封装了完善的表单功能，详情见[官方文档](https://docs.djangoproject.com/zh-hans/2.1/topics/forms/)。

 核心内容是 django 的 Form 类和其子类（ModelForm） 。

​	\# ModelForm 是将模型的字段转换成表单字段，在从表单字段生成 Html 表单控件。

```python
from django.forms import Form，ModelForm
```

工作流程： 

> \* 1.自定义 Form 类，继承自 django.forms.From 或其子类。
>
> \* 2.在视图函数中实例化自定义的 Form 类，并传给模板。
>
> \* 3.模板中根据 Form 类动态生成对应的表单元素控件。

#### 1.自定义 Form 类

  在 app 下创建form.py。如下：

```python
from django import forms

# 自定义数据验证函数
def weight_validate(value):
    if not str(value).isdigit():
        raise ValidationError('请输入正确的重量')
        
class ProductForm(forms.Form):
    # 设置错误信息并设置样式
    # help_text 提示信息
    # required 是否必须需要，默认为True
    name = forms.CharField(max_length=20, label='名字',
                           widget=forms.widgets.TextInput(attrs={'class': 'c1'}),
                           error_messages={'required': '名字不能为空'},)
    # 使用自定义数据验证函数
    weight = forms.CharField(max_length=50, label='重量',validators=[weight_validate])
    size = forms.CharField(max_length=50, label='尺寸')
    # 获取数据库数据
    choices_list = [(i+1, v['type_name']) for i,v in enumerate(Type.objects.values('type_name'))]
    # 设置CSS样式
    type = forms.ChoiceField(widget=forms.widgets.Select(attrs={'class': 'type','size':'4'}),
                             choices=choices_list,label='产品类型')
```

> \*1. forms 中定义了很多的数据类型。[所有字段](https://docs.djangoproject.com/zh-hans/2.1/ref/forms/fields/)
>
> \* 2.每个类字段对应 HTML 中的一个控件。
>
> \* 3.每个字段类型，也都有一些特定的参数，达到数据的规范型。[说明](https://docs.djangoproject.com/zh-hans/2.1/ref/forms/fields/)
>
> \* 4.**每个 Form 类都有一个 is_valid() 方法，用来验证数据，如果所有字段都包含数据，则会返回 True 并将表单数据放到它的 cleaned_data 中。**
>
> \# 不包括`submit` 控件，要在模板中实现



#### 2.实列化和模板

```python
# views.py
from .form import MyForm

def test(request):
    myform = MyForm()
    return  render(request, template_name='patients/test.html', context=locals())
```

> 一般流程：
>
> \*1.判断请求方式
>
> \* 2.创建实例
>
> > 如果模板中表单的请求方式是 post。
> >
> > 如果判断请求方式也是 post 方法， 那么实例化时要用请求数据实例化。
> >
> > ```python
> > myform = MyForm(request.POST)
> > ```
> >
> > 否则创建一个空的表单：
> >
> > ```python
> > myform = MyForm()
> > ```
>
> \* 3.使用 is_valid 验证数据。
>
> \* 4.提取数据，处理数据。表单数据在 cleaned_data 属性中。
>
> ```python
> subject = myform.cleaned_data['subject']
> ```
>
> \* 4.渲染模板



```html
# test.html

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
    {% if myform.errors %}  <!-- 渲染表单错误 -->
        <p>
            数据出错啦，错误信息：{{ myform.errors }}.
        </p>
    {% else %}

        <form action="" method="post">
            {% csrf_token %}  <!-- 使用 csrf_token 标签，防止跨站攻击 -->
            <table>
                {{ myform.as_table  }}  <!-- 使用实例化的 myform，自动创建表单控件  -->
            </table>
            <input type="submit" value="提交">
        </form>

    {% endif %}
</body>
</html>
```

> \# 记得实现提交按钮

> \* **1**.如果实例化的表单名叫 ‘form’，那么模板中 `{{ form }}`将渲染它为 Html 中相应的 `<label>` 和 `<input>` 元素。
>
> \* **2**.也可以渲染成其他标签，如下：
>
> > 1.` {{ form.as_table }}`  ：<tr> 标签
> >
> > 2.`{{ form.as_p }}` ：<p> 标签
> >
> > 3.`{{ form.as_ul }}` ： <li> 标签
> >
> > \# 但是要在模板中提供外层标签，<table> 、<ul> 等。
>
> \* **3**.同时 django 也提供了手动渲染的方式，[详情见文档](https://docs.djangoproject.com/zh-hans/2.1/topics/forms/)

