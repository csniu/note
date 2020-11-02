## django 分页

django 的分页管理对象是 [`Paginator`](https://docs.djangoproject.com/zh-hans/2.1/topics/pagination/#django.core.paginator.Paginator) 类 和 `Page` 类 。

实例化是需要两个参数，数据源和每页需要展示的条数。

```python
from django.core.paginator import Paginator
objects = ['john', 'paul', 'george', 'ringo']
p = Paginator(objects, 2)
```

<!-- more -->

Paginator 的属性和方法：

| 属性                         | 作用                                              |
| ---------------------------- | ------------------------------------------------- |
| p.count                      | 共分为多少页                                      |
| p.page_range                 | 页码生成器                                        |
| page1 = p.page(1)            | 取得第一页 Page 对象, 超出范围抛出 EmptyPage 错误 |
| page1.object_list            | 第一页的所有数据                                  |
| page2.has_next()             | 返回有没有下一页的布尔值                          |
| page2.has_previous()         | 返回有没有前一页的布尔值                          |
| page2.has_other_pages()      | 返回有没有其他页的布尔值                          |
| page2.next_page_number()     | 返回下一页的页码，如果没有会抛出EmptyPage错误     |
| page2.previous_page_number() | 前一页                                            |
| page2.start_index()          | 当前页的第一条数据 index                          |
| page2.end_index()            | 当前页最后一条 index                              |



视图函数中：

```python
from django.core.paginator import Paginator
from django.shortcuts import render

def listing(request):
    contact_list = Contacts.objects.all()
    paginator = Paginator(contact_list, 25) # 创建分页数据

    page = request.GET.get('page') 
    contacts = paginator.get_page(page)  
    return render(request, 'list.html', {'contacts': contacts})
```



模板中：

```html
{% for contact in contacts %}
    {# Each "contact" is a Contact model object. #}
    {{ contact.full_name|upper }}<br>
    ...
{% endfor %}

<div class="pagination">
    <span class="step-links">
        {% if contacts.has_previous %}
            <a href="?page=1">&laquo; first</a>
            <a href="?page={{ contacts.previous_page_number }}">previous</a>
        {% endif %}

        <span class="current">
            Page {{ contacts.number }} of {{ contacts.paginator.num_pages }}.
        </span>

        {% if contacts.has_next %}
            <a href="?page={{ contacts.next_page_number }}">next</a>
            <a href="?page={{ contacts.paginator.num_pages }}">last &raquo;</a>
        {% endif %}
    </span>
</div>
```

