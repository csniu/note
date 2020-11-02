# RESTful 框架
- RESTful框架是一种 Web 框架的**设计规范**，而不是标准。
- REST  是 `Representational State Transfer` 的缩写，翻译为“(**资源**的)表现层状态转化”。
- 资源：就是网络上的一个实体，可以是一段文本，一张图片等等。每种资源都对应一个特定的URL（统一资源定位符）。
- 表现层：资源的表现形式，可以是 XML、JSON、HTML等格式。
- URL只代表资源的实体（定位），并不代表它的表现形式。即使URL中有“.json”后缀也不能代表就是json格式的数据。它的具体表现形式，应该在HTTP请求的头信息中用Accept和Content-Type字段指定。
- 状态：
    - 实际上状态应该区分为应用状态和资源状态，客户端负责维护应用状态，而服务端维护资源状态。
    - 客户端与服务端的交互必须是无状态的（标准的（无Cookie、Session）HTTP协议是一种**无状态**（服务端不应该保存客户端状态）的协议），并在每一次请求中包含处理该请求所需的一切信息。
    - 服务端不需要在请求间保留应用状态，只有在接受到实际请求的时候，服务端才会关注应用状态。
    - 如果Cookie保存的是一些服务器不依赖于会话状态即可验证的信息（比如认证令牌），这样的Cookie也是符合REST原则的。
- （应用）状态转化：
    - "会话"状态不是作为资源状态保存在服务端的，而是被客户端作为应用状态进行跟踪的。
    - 客户端应用状态在服务端提供的超媒体的指引下发生**变迁**，服务端通过超媒体告诉客户端当前状态有哪些后续状态可以进入。
- 表现层状态转移：建立在表现层上的状态转化。客户端通过HTTP的不同动作完成资源的操作。
- 资源接口要统一，并且是要HTTP标准动作操作资源。

<!-- more -->

总结：
- 每一个URL代表一种资源
- 客户端和服务端之间，传递这种资源的某种表现形式（XML、JSON等）
- 客户端通过HTTP动作对服务端的资源进行操作，实现“表现层状态转化”


一般设计原则有如下：
- 对于 url 的定义：
    - 使用名词不使用动词
    - 使用复数
    - URL中使用连接符“-”代替下划线“_”,且使用小写
    - URL 中加入版本号
- 使用HTTP方法操作资源
    - HEAD：只获取资源的头部信息，如修改时间等。
    - GET: 获取资源，只是查询不修改资源。
    - POST：创建新的资源。
    - PUT： 更新资源的全部属性。需传递所有属性值，即使没有。
    - PATCH：更新资源的部分属性。PATCH比较新且比较复杂，一般使用POST代替。
    - DELETE：删除资源。

更新和创建返回最新的资源，删除不返回内容。
对于仅支持GET、POST方法的客户端，可以使用POST方法加头部信息模仿其他方法

|     |GET  |POST |PUT  |DELETE|
| --- | --- | --- | --- | ---  |
|/employees  |获取集合的所有资源  |创建一个新的资源  |批量更新  |删除所有  |
|/employees/1  |获取具体资源  |错误  |更新  |删除  |

- 可选、复杂的参数使用查询字符串（？）
- 返回状态码+头部文件+详细信息
    - 200 : 一般性的成功，不能用于错误返回
    - 201： 资源创建成功
    - 202：异步操作，表示已搜到当未处理
    - 400： 一般性的失败
    - 401：身份验证失败
    - 403：非验证性资源访问被禁止
    - 404：资源不存在
    - 405：HTTP方法不支持
    - 406：请求数据格式不支持
    - 500： 服务端错误

- 数据字段使用JavaScript命名规范（小写驼峰命名:yearOfBirth）
- 使用分页，而不是返回所有资源
- 限流（rate limit）


详情请参考["理解RESTful架构( 阮一峰)"](https://www.ruanyifeng.com/blog/2011/09/restful.html)、["RESTful API 最佳实践(阮一峰)"](http://www.ruanyifeng.com/blog/2018/10/restful-api-best-practices.html)、[RESTful架构](https://www.cnblogs.com/zhangxinglong/p/11113235.html)



## Django 中 RESTful API 实现
Django REST framework 是 Django 的一个第三方插件，用于快速开发 RESTful API。
具有一下特点：
- 自动生成网站API文档；
- 提供了定义序列化器Serializer的方法，可以快速根据 Django ORM 或者其它库自动序列化/反序列化；
- Authentication（身份认证）：快速实现支持Auth1、Auth2方式的权限验证。
- 提供了丰富的类视图、Mixin扩展类：简化视图的编写；
- 内置了限流系统；
- Request and Response（请求与响应）：扩展了常规的HttpResquest和TemplateResponse；
- ViewSet and Routers（视图集和路由）：让路由设置更简便；

详情见官方文档[地址](https://www.django-rest-framework.org/)。

# Django REST framework（DRF）

## 安装配置

### 安装
```
pip install djangorestframework
```
如需要其他功能，需令行安装，如：
```
pip install markdown  # browsable API
pip install django-filter # 数据过滤
```

### 配置
- 直接在 django 项目的 settings.py 中注册 app 即可：
```
INSTALLED_APPS = [
    'rest_framework'
]
```
- app 的配置信息都保存在项目的`settings.py`模块中的`REST_FRAMEWORK`字典中：
```
REST_FRAMEWORK = {
    'DEFAULT_PERMISSION_CLASSES': [
        'rest_framework.permissions.DjangoModelPermissionsOrAnonReadOnly'
    ]
}
```
- app  还提供了内置视图，只需将其添加到项目的路由中即可使用：
```
from django.contrib import admin
from django.urls import path, include, re_path
from django.conf.urls import include, url
from rest_framework.documentation import include_docs_urls

urlpatterns = [
    path('admin/', admin.site.urls),
    path('docs/', include_docs_urls(title="这是标题")),  # 文档（自动生成）
    url(r'^api-auth/', include('rest_framework.urls'))
    # re_path(r'^api-auth/', include('rest_framework.urls'))  # 2.x 版本推荐写法
```

python manage.py runserver 启动服务即可访问：
- `http://127.0.0.1:8000/api-auth/login/`: 登录
- `http://127.0.0.1:8000/docs/`：API文档
- `http://127.0.0.1:8000/admin/`：django 后台


## 模型的序列化/反序列化
django 中使用的 ORM 模型，想要实现 REST 的规范（表现层），首先就要定义模型的序列化和反序列化方法，好便于数据的传输。当然一般使用的 json 格式的数据。
在 DRF 中定义了如下类，用于模型的序列化和反序列化。
### [BaseSerializer](https://www.django-rest-framework.org/api-guide/serializers/#baseserializer)
BaseSerializer: 继承于 `rest_framework.fields.Field`， 同时也是其他序列化类的基类。
    - initial_data： 原始输入数据
    - is_valid(): 检验数据
    - errors: 验证期间的错误信息。
    - validated_data： 校验后的输入数据
    - data(): 序列化后的数据
    - save: 将验证后的数据保存到对象实例中。（create、update）
    - to_representation():  重写此方法来改变读取操作的序列化结果。
    - to_internal_value():  重写此方法来改变写入操作的序列化结果。
    - create() 和 update(): 重写其中一个或两个来改变保存实例时的动作。
    
实例化参数：
- instance： 可选，数据模型实例。
- data：可选，需要反序列化的数据。
- kwargs：其他可选关键字参数。包括 'read_only', 'write_only', 'required', 'default', 'initial', 'source','label', 'help_text', 'style', 'error_messages', 'allow_empty','instance', 'data', 'partial', 'context', 'allow_null'等。

序列化和反序列化操作：
- 只给数据实例对象：序列化。
- 只给反序列化数据：新建数据实例。
- 同时给模型实例和反序列化数据：更新实例全部数据。
    - 关键字参数 partial 为 True：更新数据部分数据。

### [Serializer](https://www.django-rest-framework.org/api-guide/serializers/#serializers)
Serializer：继承于 BaseSerializer 与 SerializerMetaclass，重写了一些方法，也是最常用的一种。BaseSerializer

- 定义对象:
```
from django.db import models

class Comment(models.Model):
    email = models.EmailField(max_length=30)
    content = models.CharField(max_length=30)
    created = models.CharField(max_length=30)
```
- 定义序列化器
 ```
from rest_framework import serializers

class CommentSerializer(serializers.Serializer):
    email = serializers.EmailField() 
    content = serializers.CharField(max_length=200)
    created = serializers.DateTimeField()
 ```
 
 \# 每一种数据类型都会有一定的序列化方法。参考[文档](https://www.django-rest-framework.org/api-guide/fields/)

- 序列化对象
```
from rest_framework.renderers import JSONRenderer

comment = Comment(email='leila@example.com', content='foo bar')  # 创建实例

serializer = CommentSerializer(comment)  # 使用自定义的序列化器序列化实例
print(serializer.data)  # 序列化成Python数据类型
\# {'email': 'leila@example.com', 'content': 'foo bar', 'created':'2016-01-27T15:17:10.375877'}

json = JSONRenderer().render(serializer.data)  # 渲染成 Json
\# '{"email":"leila@example.com","content":"foobar","created":"2016-01-27T15:17:10.375877"}'
```
 实际是调用`self.to_representation()`和`field.get_attribute()`以及`field.to_representation()`方法进行序列化操作的。
 如果需要序列化的对象是一个集合（QuerySet）时，则需在实列化是设置`many`为True。
 如果在序列化中需要额外的上下文对象，可通过`context`传入。则在序列时都会在该对象中查找上下文。


- 反序列化
```
import io
from rest_framework.parsers import JSONParser

stream = io.BytesIO(json)
data = JSONParser().parse(stream)  # 解析json数据成python数据类型

serializer = CommentSerializer(data=data)
if serializer.is_valid():  # 检验数据
    print(serializer.validated_data) 
```
在序列化时可以通过设置`many=True`实现一次序列化多个对象。在反序列化中是默认支持多对象的，但是只支持创建不支持更新。

- 数据验证
    - 反序列化过程中应该始终先调用`is_valid()`的方法进行数据验证，然后再使用其他方法操作。如果验证错误会将错误信息保存到`errors`属性中。
    - `is_valid()`可以接收一个`raise_exception`参数，如果其值为True且验证错误则会抛出`ValidationError`异常（DRF异常处理程序会自动处理，并返回404相应），否则返回False。
    - 自定义验证: 通过查看源码可以发现调用`is_valid()`方法时的整个验证流程为：`self.is_valid()`-->`self.run_validation()`-->`self.to_internal_value()`-->`field.run_validation()`-->`self.validate_{field_name}()`-->`self.validate()`。
        - 其中`field.run_validation()`的内部逻辑又有：遍历所有的验证器（可是添加或使用默认的）进行验证，但是Serializer重写了`get_validators()`方法，使遍历验证器使如果没有设置则会在`Meta`类中查找验证器（validators属性）。
        - 所有自定义验证时可以在以下环节添加：
            - 添加字段验证器：`score = IntegerField(validators=[multiple_of_ten])`(multiple_of_ten为自定义的一个验证方法)。验证某个字段。
            - 定义`validate_{field_name}`样式的方法。验证某个字段。
            - 覆盖原`validate()`方法。可验证所有字段。
            - 定义 `Meta`类的`validators`属性。可验证所有字段。
        - 验证错误应该抛出`serializers.ValidationError`异常。
        

- 保存实例
验证好数据后，想把数据保存到数据库（保存实例）。需要实现序列化器的`create()`、`update()`方法之一或两者。
    - 实例化序列化器时如果传入对象实例（instance）则调用`update()`否则调用`create()`方法。
```
class CommentSerializer(serializers.Serializer):
    email = serializers.EmailField()
    content = serializers.CharField(max_length=200)
    created = serializers.DateTimeField()

    def create(self, validated_data):
        return Comment.objects.create(**validated_data)

    def update(self, instance, validated_data):
        instance.email = validated_data.get('email', instance.email)
        instance.content = validated_data.get('content', instance.content)
        instance.created = validated_data.get('created', instance.created)
        instance.save()
        return instance

comment = serializer.save()  # 保存，如需在保存实例时同时保存一些其他的数据，直接通过关键字参数传入即可
```
当然如果不需要保存到数据库（如发送邮件），可以直接复写save方法即可。

- 数据嵌套
```
class UserSerializer(serializers.Serializer):
    email = serializers.EmailField()
    username = serializers.CharField(max_length=100)

class CommentSerializer(serializers.Serializer):
    user = UserSerializer()  # 嵌套
    content = serializers.CharField(max_length=200)
    created = serializers.DateTimeField()
```
DRF是支持嵌套的，但是需要在编写`create()`、`update()`方法使其支持嵌套。无疑这样也增加了复杂度。（第三方软件包 DRF Writable Nested 支持自动可写嵌套表示）



### [ModelSerializer](https://www.django-rest-framework.org/api-guide/serializers/#modelserializer)
继承于 Serializer，但是其已经实现了一些功能可用于快速定义。如下：

- 根据指定的Model自动检测并生成序列化的字段，不需要提前定义；
- 自动为序列化生成校验器；
- 自动实现了create()方法和update()方法。
```
class AccountSerializer(serializers.ModelSerializer):
    url = serializers.CharField(source='get_absolute_url', read_only=True)  # 替换或增加字段
    
    class Meta:
        model = Account  # 指定模型
        fields = ['id', 'account_name', 'users', 'created']
        depth = 1
        read_only_fields = ('account_name',)
```
- 默认会序列化模型的所有字段。或者指定`filed='__all__'`
- 建议通过 fields 显示定义需要序列化的字段。或通过 exclude 定义不需要序列化的字段。
- depth：表示嵌套数据的深度。
- read_only_fields：相当于将每个字段的`read_only=True`。



### [ListSerializer](https://www.django-rest-framework.org/api-guide/serializers/#listserializer)
继承于`BaseSerializer`,通常不会直接使用。当传入`many=True`时就会创建一个`ListSerializer`实例并返回。


### [HyperlinkedModelSerializer](https://www.django-rest-framework.org/api-guide/serializers/#hyperlinkedmodelserializer)
超链接模型序列化器，继承于 BaseSerializer。类似于`ModelSerializer`当不同之处是使用超链接表示关联关系而不是使用主键。


## 视图
有了数据的序列化和反序列化后，接下来需要做的就是接收数据和返回数据了。
### [Request](https://www.django-rest-framework.org/api-guide/requests/#requests)
Request 是 DRF 扩展 HttpRequest 而成。增加了灵活的请求解析和认证功能。
- `.data`：解析后的数据。
- `.query_params`: 查询参数
- `.user`
- `.auth`
- `authenticators`
- `method`: 请求方法
等

### [Responses](https://q1mi.github.io/Django-REST-framework-documentation/api-guide/responses_zh/#responses)
- 继承于 django 的 `SimpleTemplateResponse`。可以根据标准的HTTP[内容协商](
https://www.django-rest-framework.org/api-guide/content-negotiation/)来确定应如何呈现最终响应内容。也就是说可以自动根据不同的请求返回不同类型的数据（json、xml等）。
- 无法直接渲染 Django 模型实例的复杂数据类型，但是可以使用 Serializer 序列化数据后使用。
```
Response(data, status=None, template_name=None, headers=None, content_type=None)
```
- data：返回数据
- status：状态码
- template_name: 模板名
- headers: 头信息
- content_type: 数据类型，一般会自动设定。

### 状态码
直接返回裸状态码可读性较差。DRF 定义了一组命名常量用状态的返回。
定义在 rest_framework.status 模块中。
```
from rest_framework.status import HTTP_400_BAD_REQUEST
```

### 原生版视图
仅使用了序列化类，当然也可以不使用。自定义序列化也是可以的。如下：
可以看出比较繁琐，要处理不同的请求方法和返回不同类型的数据。而且无法复用。
```
def snippet_detail(request, pk):
    try:
        snippet = Snippet.objects.get(pk=pk)
    except Snippet.DoesNotExist:
        return HttpResponse(status=404)
        
    if request.method == 'GET':
        serializer = SnippetSerializer(snippet)  # 序列化
        return JsonResponse(serializer.data)
        
    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = SnippetSerializer(snippet, data=data)  # 创建实例
        if serializer.is_valid():
            serializer.save()
            return JsonResponse(serializer.data)
        return JsonResponse(serializer.errors, status=400)
        
    elif request.method == 'DELETE':
        snippet.delete()  # 删除
        return HttpResponse(status=204)
```
### 视图类
- 为了使代码的复用和易于管理，可以使用 Django 的视图类（View）。通过多重继承的方法可以最大限度的复用代码。而且不同的请求方法对应不同的类方法，更加易于管理。
- DFR 中最基础的视图类是`APIView`，继承于 `Django` 的`View`(`django.views.generic.View`)类。但是在其基础上增加了一些功能。
- 通过查看其`dispatch`方法可知：
    - 在进入对应方法之前，会进行身份的认证和权限的检测。（self.initial）
    - 将原`request`对象转换成`Request`对象。(self.initialize_request)
    - 将原`response`对象转换成`Response`对象。（self.finalize_response）
    - 做了异常处理，并返回合适的相应。(self.handle_exception)
```
from snippets.models import Snippet
from snippets.serializers import SnippetSerializer
from django.http import Http404
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status

class SnippetDetail(APIView):
    def get_object(self, pk):
        try:
            return Snippet.objects.get(pk=pk)
        except Snippet.DoesNotExist:
            raise Http404

    def get(self, request, pk, format=None):  # GET
        snippet = self.get_object(pk)
        serializer = SnippetSerializer(snippet)
        return Response(serializer.data)

    def put(self, request, pk, format=None):  # PUT
        snippet = self.get_object(pk)
        serializer = SnippetSerializer(snippet, data=request.data)
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

    def delete(self, request, pk, format=None):  # DELETE
        snippet = self.get_object(pk)
        snippet.delete()
        return Response(status=status.HTTP_204_NO_CONTENT)
```
DRF 还提供了一个 `rest_framework.decorators.api_view` 装饰提，用于快速的修改老的视图函数。其本质也是创建一个`APIView`对象。
```
urlpatterns = [
    path('snippets/', views.SnippetList.as_view()),
    path('snippets/<int:pk>/', views.SnippetDetail.as_view()),
]
```
### 通用视图类
- `APIView`只是做了一些基本的操作并没有涉及数据模型的操作。但是作为一个API处理数据是很常见的操作。
- 为此 DRF 定义了`GenericAPIView`（`rest_framework.views`模块中）通用视图类，和一些不同动作的类（`rest_framework.generics`模块中）。通过其不同的组合就可以形成不同的视图类。
#### [GenericAPIView](https://q1mi.github.io/Django-REST-framework-documentation/api-guide/generic-views_zh/#genericapiview)
属性：
- `queryset`: 被查询数据的集合
- `serializer_class`: 序列化类
- `lookup_field`：用于查找数据的字段，默认是”pk“即数据库中”id“。
- `lookup_url_kwarg`：用于查找URL的关键字参数。默认同`lookup_field`
- `pagination_class`: 分页类
- `filter_backends`: 用于过滤查询集的过滤器后端类的列表

方法：
- `get_queryset(self)`：获取被查询数据的集合，一般要重写或赋值`queryset`属性。
- `get_objject(self)`：查询方法。使用`lookup_field`字段过滤数据。
- `get_serializer_class(self)`：过滤`queryset`方法一个新的`queryset`集合。默认`
serializer_class`属性值。
- 钩子方法：
        - `perform_create(self, serializer)`： 在保存新对象实例时由`CreateModelMixin`调用。
        - `perform_update(self, serializer)`： 在保存现有对象实例时由`UpdateModelMixin`调用。
        - `perform_destroy(self, instance)`： 在删除对象实例时由`DestroyModelMixin`调用。
- 其他方法
    - `get_serializer_context()`： 返回包含应该提供给序列化程序的任何额外上下文的字典。默认包含`request`,`view`和`format` 这些keys。
    - `get_serializer()`： 返回一个序列化器的实例。
    - `get_paginated_response()`: 返回分页样式的`Response`对象。
    - `paginate_queryset()`: 如果需要分页查询，返回页面对象，如果没有为此视图配置分页，则返回 None。
    - `filter_queryset()`: 给定查询集，使用任何过滤器后端进行过滤，返回一个新的查询集。


#### [Mixins](https://q1mi.github.io/Django-REST-framework-documentation/api-guide/generic-views_zh/#mixins)
- `ListModelMixin`: list方法，列出结果集。
- `CreateModelMixin`: create方法，创建实例。
- `RetrieveModelMixin`: retrieve方法，查询。partial_update 局部更新。
- `UpdateModelMixin`：update方法，更新。
- `DestoryModelMixin`：destory方法，删除。

#### 组合类：
通过组合上面的类，可获得支持不同功能的视图类。如`rest_framework.generics.CreateAPIView`就是继承于`mixins.CreateModelMixin``和GenericAPIView`。

### [视图集（viewset）](https://www.django-rest-framework.org/api-guide/viewsets/#viewsets)
一般和 `Routers` 结合使用，可以快速注册路由。

- 将一组视图定义在一个类中。（这一组视图不一定对应同一个url）
- 不定义任何方法处理程序（`get()`、`post()`）。但是定义`list()`、`create()`等方法（结合`Routers`使用）。

查看源码（`reset_framework.viewset`）可知，`ViewSet`继承至`ViewSetMixin`和`views.APIView`。而`ViewSet`的不同之处就在于`ViewSetMixin`中重写了`as_view()`方法并添加了`actions`参数，使之可以根据`actions`参数动态的将不同方法绑定到不同的方法处理程序上。如下：
```
from django.contrib.auth.models import User
from django.shortcuts import get_object_or_404
from myapps.serializers import UserSerializer
from rest_framework import viewsets
from rest_framework.response import Response

class UserViewSet(viewsets.ViewSet):
    """
    A simple ViewSet for listing or retrieving users.
    """
    def list(self, request):
        queryset = User.objects.all()
        serializer = UserSerializer(queryset, many=True)
        return Response(serializer.data)

    def retrieve(self, request, pk=None):
        queryset = User.objects.all()
        user = get_object_or_404(queryset, pk=pk)
        serializer = UserSerializer(user)
        return Response(serializer.data)
```
没有定义`get()`方法也就不能响应`get`请求，但是可通过`actions`参数将`list()`绑定到`get()`方法上。如源码中定义：
```
\# ViewSetMixin.as_view()
for method, action in actions.items():
    handler = getattr(self, action)
    setattr(self, method, handler)
```
配置如下：
```
user_list = UserViewSet.as_view({'get': 'list'})
```
- 当然一般不需要我们这样手动配置，而是结合 `Routers` 自动生成。
- 但是手动配置的逻辑会更加清晰。


类似于`APIView`于`GenericAPIView`，DFR 也定义了不同类组合的`'viewset'`，如`GenericViewSet`、`ReadOnlyModelViewSet`、`ModelViewSet`。详情请参考文档。
到这里你可能会有疑惑，这也只是换了一个方法名而已，为什么要这么折腾呢？
其实它的真正实力要结合`Routers`才能发挥出来。

#### [Routers](https://www.django-rest-framework.org/api-guide/routers/)
可是结合`ViewSet`它会自动匹配生产路由配置（list， detail，format 不同的url）。
使用实例：
```
from rest_framework import routers

router = routers.SimpleRouter()
router.register(r'users', UserViewSet)
router.register(r'accounts', AccountViewSet)
urlpatterns = router.urls
```

现在`urlpatterns`就可以匹配如下 url 了：

| URL 模式 | URL 名 |
| --- | --- |
| ^users/$| user-list|
| ^users/{pk}/$ | user-detail|
| ^accounts/$ | account-list|
| ^accounts/{pk}/$ | account-detail|

那它是怎么做到的呢，通过查看`rest_framework.routers.BaseRouter.urls()`可是，是由`get_urls()`方法计算得出。在查看`SimpleRouter`类的`routes`属性和`get_urls()`方法可知，会自动在`viewset`类中查找`list`、`create`等方法并绑定到不同方法处理程序上，而且还会生产不同的url，如下表（`SimpleRouter`）：
|URL 模式|HTTP 方法| 对应类方法| URL 名|
|--|--|--|--|
|{prefix}/|GET|list|{bansename}-list|
|  |POST|create|
|{prefix}/{methodname}/|GET 或由“method”的参数指定|被`@action（detail = False）`装饰的方法|{basename}-{methodname}|
|{uprefix}/{lookup}/|GET|retrieve|{basename}-detail|
| |PUT|update| |
| |PATCH|partial_update| |
| |DELETE|destroy| |
|{prefix}/{lookup}/{methodname}/|GET 或由“method”的参数指定|被`@action（detail = True）`装饰的方法|{basenmae}-{methodname}|

- prefix（url前缀）为注册时的`prefix`参数。
- `basename` 默认会根据类 `queryset` 属性自动生成（`queryset.model._meta.object_name.lower()`）。但是如果该类没有`queryset`属性（自定义`get_queryset`方法）则必须在注册时指定。
- `methodname`:被装饰的方法名。
- lookup：过滤字段，默认是"pk"。
- action 装饰器定义在 `decorators`模块中（定义了所有的装饰器）。

`DefaultRouter`与`SimpleRouter`的不同之处是可以匹配url的后缀，生成不同的类型的数据。
如`{prefix}.json`则可以生成json数据。（集成了[format_suffix_patterns](https://q1mi.github.io/Django-REST-framework-documentation/api-guide/format-suffixes/#format_suffix_patterns)）

##### 使用 urls
- 直接附加到原`urlpatterns`中
```
router = routers.SimpleRouter()
router.register(r'users', UserViewSet)
router.register(r'accounts', AccountViewSet)

urlpatterns = [
    url(r'^forgot-password/$', ForgotPasswordFormView.as_view()),
]

urlpatterns += router.urls
```

- include
```
urlpatterns = [
    url(r'^forgot-password/$', ForgotPasswordFormView.as_view()),
    url(r'^', include(router.urls), namespace),  # 支持命名空间
]
```

### 响应不同格式数据
前面说 `Response` 时说到它可以根据[请求头`Accept`](
https://www.django-rest-framework.org/api-guide/content-negotiation/)响应不同格式的数据，比如 `json`、`xml `等等，但是前提是要先安装对应的渲染器并且在`settings.REST_FRAMEWORK`中启用才行，详情见[文档](
https://www.django-rest-framework.org/api-guide/renderers/)，这里以 `xml` 为例。
```
pip install djangorestframework-xml
```
```
REST_FRAMEWORK = {
    'DEFAULT_RENDERER_CLASSES': [
        'rest_framework.renderers.JSONRenderer',
        'rest_framework_xml.renderers.XMLRenderer',
        ]
}
```
#### `Accept`
现在如果你的视图返回的是`Response`类，就可以响应不同类型的数据了。如下：
```
import requests

requests.get('http://127.0.0.1:8000/tasks/', headers={'Accept': 'application/xml'})
requests.get('http://127.0.0.1:8000/tasks/', headers={'Accept': 'application/json'})
```

#### 查询字符串
DRF 默认还支持通过查询字符串来请求不同类型的数据，如下：
```
import requests

requests.get('http://127.0.0.1:8000/tasks/?format=json')
requests.get('http://127.0.0.1:8000/tasks/?format=xml')
```
\# 可以通过设置`URL_FORMAT_OVERRIDE`来修改此查询参数的名称。将该值设置None为禁用此行为。

#### 后缀名
更加直观的操作，可以通过匹配 url 的后缀名来响应不同的格式数据。如下：
```
import requests

requests.get('http://127.0.0.1:8000/tasks.json')
requests.get('http://127.0.0.1:8000/tasks.xml')
```
这需要我们对原视图和路由做如下修改：
- 在视图函数中添加`format`参数。
```
def tasks(request, format=None)
    pass
```

- 使用[`format_suffix_patterns`](
https://www.django-rest-framework.org/api-guide/format-suffixes/)修改路由
```
from django.urls import path
from rest_framework.urlpatterns import format_suffix_patterns
from .views import *

urlpatterns = [
    path('task/', tasks),
    ]
    
urlpatterns = format_suffix_patterns(urlpatterns)
```

### [分页](https://www.django-rest-framework.org/api-guide/pagination/)
#### 全局配置分页
可以通过 `settings.py` 模块中的 `REST_FRAMEWORK`，来配置全局的分页。如下：
```
REST_FRAMEWORK = { 
    'DEFAULT_PAGINATION_CLASS': 'rest_framework.pagination.LimitOffsetPagination',  # 分页类
    'PAGE_SIZE': 10   # 页面大小
    }
```
#### 局部分页
上面的方法会使所有的请求都分页，这可能并不是我们想要的。
可以通过自定义分页类来实现局部的分页。
```
from rest_framework.pagination import PageNumberPagination

\# 自定义分页类
class SnippetPagination(PageNumberPagination):
    page_size = 1
    page_size_query_param = "page_size"
    page_query_param = "p"#?p=1，默认为page
    max_page_size = 20
    
\# 修改
class SnippetDetail(mixins.RetrieveModelMixin,
                    mixins.UpdateModelMixin,
                    mixins.DestroyModelMixin,
                    generics.GenericAPIView):
    queryset = Snippet.objects.all()
    serializer_class = SnippetSerializer
    pagination_class = SnippetPagination  # 指定分页类
    ...
```

### [过滤与排序（查询集）](https://www.django-rest-framework.org/api-guide/filtering/)
列表视图默认是返回模型的全部数据，如果需要限制数据可以直接复写视图类的`get_queryset()`方法即可。

根据用户过滤：
```
  def get_queryset(self):
        user = self.request.user
        return Purchase.objects.filter(purchaser=user)
```
根据 url 参数过滤
```
url('^purchases/(?P<username>.+)/$', PurchaseList.as_view()),  # url 中包括一个 username 参数
```
```

def get_queryset(self):
    username = self.kwargs['username']
    return Purchase.objects.filter(purchaser__username=username)
```
根据查询字符串过滤
url：`http://example.com/api/purchases?username=denvercoder9`
```
def get_queryset(self):
    queryset = Purchase.objects.all()
    username = self.request.query_params.get('username', None)
    if username is not None:
        queryset = queryset.filter(purchaser__username=username)
    return queryset
```
但是这些方式都比较格式化，不好扩展也不灵活。DFR 中有一种更加通用的方式：过滤器。
可以配置全局的也可以指定区部的过滤器。在每个请求查询前会先调用过滤器的`filter_queryset()`方法过滤原查询集。

- 设置全局过滤器
```
REST_FRAMEWORK = {
    'DEFAULT_FILTER_BACKENDS': ('django_filters.rest_framework.DjangoFilterBackend',)
    }
```
- 指定视图类的过滤器
```
class UserListView(generics.ListAPIView):
    queryset = User.objects.all()
    serializer_class = UserSerializer
    filter_backends = (django_filters.rest_framework.DjangoFilterBackend,)
```
#### [DjangoFilterBackend](https://q1mi.github.io/Django-REST-framework-documentation/api-guide/filtering_zh/#djangofilterbackenddjango)
需要另行安装和注册：
- 安装
```
pip install django-filter
```
- 注册
```
INSTALLED_APPS = [ 
    ...
    'django_filters'
    ]
```
- 使用
    - 全局
    ```
    REST_FRAMEWORK = {
        'DEFAULT_FILTER_BACKENDS': ['django_filters.rest_framework.DjangoFilterBackend']
        }
    ```
    - 局部
    ```
    class ProductList(generics.ListAPIView):
        queryset = Product.objects.all()
        serializer_class = ProductSerializer
        filter_backends = [DjangoFilterBackend]
        filterset_fields = ['category', 'in_stock']
    ```
   - url
   ```
  http://example.com/api/products?category=clothing&in_stock=True
  ```
#### 另外 DRF 内置了如下过滤器(需添加到filter_backends中使用)
- [rest_framework.filters.SearchFilter](https://www.django-rest-framework.org/api-guide/filtering/#searchfilter)：查询过滤，search_fields 设置查询字段。搜索参数名默认为'search'。
- [rest_framework.filters.OrderingFilter](https://www.django-rest-framework.org/api-guide/filtering/#orderingfilter)： 排序过滤，ordering_fields 设置排序字段。查询参数名默认为'ordering'。

当然也可自定义过滤器和使用第三方的过滤，详情查看官方文档。


## 身份认证
如果你使用的视图类，你可能会发现在 `APIView`类的`initial()`方法中有如下代码：
```
self.perform_authentication(request)
self.check_permissions(request)
self.check_throttles(request)
```
由此可知，对于所有的请求都会在执行其他操作之前，依次进行：身份验证、权限检查、节流控制操作。但是身份认证本身不会允许或不允许传入的请求，它只会标识发出请求的凭据（`
request.user`、`request.auth`）。
- `request.user`: `User`类的实例
- `request.auth`: 任何身份认证信息，如身份验证令牌。

确认身份验证
- 身份验证方案始终定义为类列表，DRF 将尝试对列表中的每个类进行身份验证，并使用**第一个**身份验证成功的类的返回值设置`request.user`、`request.auth`。
- 都不通过：`request.user`设置为`django.contrib.auth.models.AnonymousUser`，`request.auth`设置为`None`（可以使用`UNAUTHENTICATED_USER`和`UNAUTHENTICATED_TOKEN`设置进行修改）。

DRF 内置了一些认证方案，定义在`rest_framework.authentication`模块中。
使用认证方案：
- 全局配置：
```
REST_FRAMEWORK = {
    'DEFAULT_AUTHENTICATION_CLASSES': (
        'rest_framework.authentication.BasicAuthentication',
        'rest_framework.authentication.SessionAuthentication',
    )}
```
- 局部配置
```
from rest_framework.authentication import SessionAuthentication, BasicAuthenticationfrom rest_framework.permissions import IsAuthenticatedfrom rest_framework.response import Responsefrom rest_framework.views import APIView

class ExampleView(APIView):
    authentication_classes = (SessionAuthentication, BasicAuthentication)
    permission_classes = (IsAuthenticated,)

    def get(self, request, format=None):
        content = {
            'user': unicode(request.user),  # `django.contrib.auth.User` 实例。
            'auth': unicode(request.auth),  # None
        }
        return Response(content)
```


```
@api_view(['GET'])@authentication_classes((SessionAuthentication, BasicAuthentication))@permission_classes((IsAuthenticated,))def example_view(request, format=None):
    content = {
        'user': unicode(request.user),  # `django.contrib.auth.User` 实例。
        'auth': unicode(request.auth),  # None
    }
    return Response(content)
```
响应状态码：
401：未认证，必须始终包括一个WWW-Authenticate头。
403：无权限

#### 认证方案
##### 内置方案
- [BasicAuthentication](https://www.django-rest-framework.org/api-guide/authentication/#basicauthentication): 基于用户名密码认证。
- [TokenAuthentication](https://www.django-rest-framework.org/api-guide/authentication/#tokenauthentication): 基于`Token`认证。
- [SessionAuthentication](https://www.django-rest-framework.org/api-guide/authentication/#sessionauthentication): Django默认的会话身份验证。
- [RemoteUserAuthentication](https://www.django-rest-framework.org/api-guide/authentication/#remoteuserauthentication): 服务器认证。

##### 第三方方案
-  [Django OAuth Toolkit](https://www.django-rest-framework.org/api-guide/authentication/#django-oauth-toolkit): OAuth 2.0 认证。[官网](
https://jwt.io/introduction/)
- [JSON Web Token Authentication](https://www.django-rest-framework.org/api-guide/authentication/#json-web-token-authentication): Token
等等

##### 方案选择
对于前后端分离的开发模式，一般会使用`TokenAuthentication`，但是它本身也会有问题，如在分布式系统中同步问题，以及Token 过期时间等问题。而现在更多的是采用`JSON Web Token Authentication`认证方案。具体采用什么方案还要根据实际分析可自行 Google，这里就不深究了。


## [权限](https://www.django-rest-framework.org/api-guide/permissions/)
验证了身份之后就需要验证权限了，权限检查通常会使用`request.user`和`request.auth`属性中的身份验证信息来确定是否允许传入请求。
如果验证失败会做如下响应：（`APIView.handle_exception()`）
- 403：身份通过验证，但权限被拒绝
- 403: 未通过身份验证，并且最高优先级的身份验证类不使用WWW-Authenticate标头。
- 401，WWW-Authenticate: 未通过身份验证，并且最高优先级的身份验证类确实使用WWW-Authenticate标头。

前面说到在视图类的`initial()`方法中会调用其`self.check_permissions()`方法检测用户权限。DRF 还有一次对象（模型）级别的权限检查，在调用`get_object()`(`GenericAPIView`)方法时会调用其`check_object_permissions()`方法检测权限，如`RetrieveModelMixin.retrieve()`中。

DRF 内置一些权限验证的方案，定义在`rest_framework.permissions`模块中。如果需要自定义可以继承其`BasePermission`类，并重写其`has_permission()`(`heck_permissions()`调用）和`has_object_permission()`（`check_object_permissions()`调用）方法即可。

内置的方案有：
- `AllowAny`：允许所有请求。
- `IsAuthenticated`：只允许通过身份认证的请求。
- `IsAdminUser`：只允许`user.is_staff`为True的用户访问。
- `IsAuthenticatedOrReadOnly`：通过身份认证的请求或者请求方法为('GET', 'HEAD', 'OPTIONS'）的请求。
- `DjangoModelPermissions`：django 的权限管理，并且视图类要有`quertset`属性。
    - POST：需要用户对模型有”添加“权限。
    - PUT、PATCH：需要用户对模型有”更改“权限。
    - DELETE：需要用户对模型有”删除“权限。
- `DjangoModelPermissionsOrAnonReadOnly`：类似`DjangoModelPermissions`，但允许未通过身份认证用户的只读请求。
- `DjangoObjectPermissions`

还有一些第三方的可供选择，或者自定义也行。

全局配置：
```
REST_FRAMEWORK = {
    'DEFAULT_PERMISSION_CLASSES': (
        'rest_framework.permissions.IsAuthenticated',
    )}
```
局部配置：
```
class ExampleView(APIView):
    permission_classes = (IsAuthenticated,)
    ....
```

## [流控](https://www.django-rest-framework.org/api-guide/throttling/)
流控类似与权限校验，在处理请求之前都会进行流控检查（`check_throttles()`），如果未进行限制则正常返回并更新当前用户的状态，如果限制请求则会引发`exceptions.Throttled`异常，并返回`429`状态码和包括详细信息（`Retry-After`, `detail`）的头部数据。

参看源码可知：
- 调用流控类的`allow_request()`方放检查是否限制。
- 调用流控类的`wait()`方法计算下一次的请求时间。
- 默认使用 Django 的缓存后端，所有要确保已设置必要的**缓存设置**。[django文档](https://docs.djangoproject.com/en/stable/ref/settings/#caches)
- `get_ident()`方法用于识别用户，一般不需要修改。

配置：
```
REST_FRAMEWORK = {
    'DEFAULT_THROTTLE_CLASSES': (  # 限流方案
        'rest_framework.throttling.AnonRateThrottle',
        'rest_framework.throttling.UserRateThrottle'
    ),
    'DEFAULT_THROTTLE_RATES': {  # 限流策略
        'anon': '30/minute',  # 可选单位有 second，minute，hour和day。
        'user': '60/minute'
    }
}
```

内置方案：
- `AnonRateThrottle`：仅限制未经授权的用户，并使用`anon`策略。
- `UserRateThrottle`：限制所有用户请求，并使用`user`策略。
- `ScopedRateThrottle`：仅当视图类有`throttle_scope`属性时才应用此方案。使用`
throttle_scope`属性值的方案。

\# 不同的方案会有不同默认的策略，为了使相同的方案使用不同的策略，可以通过继承重写`scope`属性，从而使用不同的策略。如下：
```
REST_FRAMEWORK = {
    'DEFAULT_THROTTLE_CLASSES': [
        'example.throttles.BurstRateThrottle',
        'example.throttles.SustainedRateThrottle'
    ],
    'DEFAULT_THROTTLE_RATES': {
        'burst': '60/min',
        'sustained': '1000/day'
    }}
```
```
class BurstRateThrottle(UserRateThrottle):
    scope = 'burst'
    
class SustainedRateThrottle(UserRateThrottle):
    scope = 'sustained'
```

## 跨域访问
同源：指两个URl的"协议+域名+端口”完全相同。

访问场景：
- 登陆网站A，生成本地Cookie。
- 在不退出A的情况下，登陆网站B。
- 网站B发起对网站A的请求，此时浏览器会自动带上网站A的`Cookie`。（拿到了A的登陆信息）

为了防止这种情况，浏览器会执行一种安全策略：[同源策略](https://developer.mozilla.org/zh-CN/docs/Web/Security/Same-origin_policy)。来限制不同源资源的交互。

同源策略的含义：
- DOM 层面的同源策略：限制了来自不同源的”`Document`”对象或 JS 脚本，对当前“`document`”对象的读取或设置某些属性
- `Cookie`和X`MLHttprequest`层面的同源策略：禁止 `Ajax` 直接发起跨域HTTP请求（其实可以发送请求，结果被浏览器拦截，不展示），同时 `Ajax` 请求不能携带与本网站不同源的 Cookie。
- 同源策略的非绝对性：`<script>、<img>、<iframe>、<link>、<video>、<audio>`等带有src属性的标签可以从不同的域加载和执行资源。

跨域：当协议、子域名、主域名、端口号中任意一个不相同时，都算作不同域。不同域之间相互请求资源，就是“跨域”了。
如何正确的跨域就需要一定的策略了，常见的方法如下：
- `jsonp`：只支持GET请求
- `cors`：支持所有HTTP请求
- Node中间件代理
- nginx反向代理

一般使用 `cors` 和 nginx反向代理方案的比较多，这里不做深究，具体操作可以自行 Google。


## 参考
- 官方文档：https://www.django-rest-framework.org/
- 中文文档：https://q1mi.github.io/Django-REST-framework-documentation/
- 九种跨域方式实现原理（完整版）：https://juejin.im/post/5c23993de51d457b8c1f4ee1
- 不要再问我跨域的问题了：https://segmentfault.com/a/1190000015597029