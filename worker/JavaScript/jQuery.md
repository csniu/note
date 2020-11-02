jQuery
jQuery 是 JavaScript 的的一个库。可以从官网下载，就是一个 xxx.js 文件。
在网页的 <head> 中导入即可使用，<script src="//code.jquery.com/jquery-1.11.3.min.js"></script>
jQuery把所有功能全部封装在一个全局变量jQuery中，而$也是一个合法的变量名，它是变量jQuery的别名。
$本质上就是一个函数，但是函数也是对象，于是$除了可以直接调用外，也可以有很多其他属性。
jQuery这么流行，肯定是因为它解决了一些很重要的问题。实际上，jQuery能帮我们干这些事情：
消除浏览器差异：你不需要自己写冗长的代码来针对不同的浏览器来绑定事件，编写AJAX等代码；
简洁的操作DOM的方法：写$('#test')肯定比document.getElementById('test')来得简洁；
轻松实现动画、修改CSS等各种操作。
选择器
id
// 查找<div >:
var div = $('#abc');

tag
var ps = $('p'); // 返回所有<p>节点
ps.length; // 数一数页面有多少个<p>节点

class
var a = $('.red'); // 所有节点包含``都将返回
var a = $('.red.green'); // 所有同时包含red和green的节点
var tr = $('tr.red'); // 找出<tr >...</tr>

属性
var email = $('[name=email]'); // 找出<??? name="email">
var icons = $('[name^=icon]'); // 找出所有name属性值以icon开头的DOM
var names = $('[name$=with]'); // 找出所有name属性值以with结尾的DOM
var emailInput = $('input[name=email]');  // 只查找<input>
var icons = $('[class^="icon-"]'); // 找出所有class包含至少一个以`icon-`开头的DOM

多项选择（和）
// 多项选择器就是把多个选择器用,组合起来一块选：
$('p,div'); // 把<p>和<div>都选出来
$('p.red,p.green'); // 把<p >和<p >都选出来

层级选择、子选择器
!-- HTML结构 -->
<div >
    <ul >
        <li >JavaScript</li>
        <li >Python</li>
        <li >Lua</li>
    </ul>
</div>

// 层级之间用空格隔开。
$('ul.lang li.lang-javascript'); // [<li >JavaScript</li>]
$('div.testing li.lang-javascript'); // [<li >JavaScript</li>]
$('ul.lang li');  // 所有的<li>节点

// 使用“>”隔开，限定了层级关系必须是父子关系，就是<child>节点必须是<parent>节点的直属子节点。
$('ul.lang>li.lang-javascript'); // 可以选出[<li >JavaScript</li>]
$('div.testing>li.lang-javascript'); // [], 无法选出，因为<div>和<li>不构成父子关系

过滤器
// 过滤器一般不单独使用，它通常附加在选择器上，帮助我们更精确地定位元素.
// 使用 “:”

$('ul.lang li'); // 选出JavaScript、Python和Lua 3个节点

$('ul.lang li:first-child'); // 仅选出JavaScript
$('ul.lang li:last-child'); // 仅选出Lua
$('ul.lang li:nth-child(2)'); // 选出第N个元素，N从1开始
$('ul.lang li:nth-child(even)'); // 选出序号为偶数的元素
$('ul.lang li:nth-child(odd)'); // 选出序号为奇数的元素

表单相关
针对表单元素，jQuery还有一组特殊的选择器：

:input：可以选择<input>，<textarea>，<select>和<button>；

:file：可以选择<input type="file">，和input[type=file]一样；

:checkbox：可以选择复选框，和input[type=checkbox]一样；

:radio：可以选择单选框，和input[type=radio]一样；

:focus：可以选择当前输入焦点的元素，例如把光标放到一个<input>上，用$('input:focus')就可以选出；

:checked：选择当前勾上的单选框和复选框，用这个选择器可以立刻获得用户选择的项目，如$('input[type=radio]:checked')；

:enabled：可以选择可以正常输入的<input>、<select> 等，也就是没有灰掉的输入；

:disabled：和:enabled正好相反，选择那些不能输入的。

查找和过滤方法
通常情况下选择器可以直接定位到我们想要的元素，但是，当我们拿到一个jQuery对象后，还可以以这个对象为基准，进行查找和过滤。
<!-- HTML结构 -->
<ul >
    <li >JavaScript</li>
    <li >Python</li>
    <li >Swift</li>
    <li >Scheme</li>
    <li name="haskell">Haskell</li>
</ul>

// find(),向下
var ul = $('ul.lang'); // 获得<ul>
var dy = ul.find('.dy'); // 获得JavaScript, Python, Scheme
var swf = ul.find('#swift'); // 获得Swift
var hsk = ul.find('[name=haskell]'); // 获得Haskell

// parent()， 向下
var swf = $('#swift'); // 获得Swift
var parent = swf.parent(); // 获得Swift的上层节点<ul>
var a = swf.parent('.red'); // 获得Swift的上层节点<ul>，同时传入过滤条件。如果ul不符合条件，返回空jQuery对象

// 同级
var swift = $('#swift');

swift.next(); // Scheme
swift.next('[name=haskell]'); // 空的jQuery对象，因为Swift的下一个元素Scheme不符合条件[name=haskell]

swift.prev(); // Python
swift.prev('.dy'); // Python，因为Python同时符合过滤器条件.dy

// filter(),可以接收一个选择器或一个函数。过滤掉不符合的节点。

var langs = $('ul.lang li'); // 拿到JavaScript, Python, Swift, Scheme和Haskell
var a = langs.filter('.dy'); // 拿到JavaScript, Python, Scheme

var langs = $('ul.lang li'); // 拿到JavaScript, Python, Swift, Scheme和Haskell
var arr = langs.map(function () {
    return this.innerHTML;
}).get(); // 用get()拿到包含string的Array：['JavaScript', 'Python', 'Swift', 'Scheme', 'Haskell']


此外，一个jQuery对象如果包含了不止一个DOM节点，first()、last()和slice()方法可以返回一个新的jQuery对象，把不需要的DOM节点去掉：
var langs = $('ul.lang li'); // 拿到JavaScript, Python, Swift, Scheme和Haskell
var js = langs.first(); // JavaScript，相当于$('ul.lang li:first-child')
var haskell = langs.last(); // Haskell, 相当于$('ul.lang li:last-child')
var sub = langs.slice(2, 4); // Swift, Scheme, 参数和数组的slice()方法一致

操作DOM
拿到jQuery对象后就是要操作DOM了。
jQuery 对象有 text()、html()方法用来获取节点的文本和原始HTML文本。
text()、html()方法无参数时表示获取文本，有参数时表示设置文本。
jQuery 对象可以包括0个或任意个DOM对象，text()、html()方法会作用到所有节点上，及时没有也不会报错。
<!-- HTML结构 --><ul >
    <li >JavaScript</li>
    <li name="book">Java & JavaScript</li></ul>

$('#test-ul li[name=book]').text(); // 'Java & JavaScript'
$('#test-ul li[name=book]').html(); // 'Java & JavaScript'

css(''name', 'value') 方法可以修改jQuery对象的css属性。
$('#test-css li.dy>span').css('background-color', '#ffd351').css('color', 'red');

var div = $('#test-div');
div.css('color'); // '#000033', 获取CSS属性
div.css('color', '#336699'); // 设置CSS属性
div.css('color', ''); // 清除CSS属性
// 为了和JavaScript保持一致，CSS属性可以用'background-color'和'backgroundColor'两种格式。

修改class属性
var div = $('#test-div');
div.css('color'); // '#000033', 获取CSS属性
div.css('color', '#336699'); // 设置CSS属性
div.css('color', ''); // 清除CSS属性

显示和隐藏DOM
// 要隐藏一个DOM，我们可以设置CSS的display属性为none，利用css()方法就可以实现。不过，要显示这个DOM就需要恢复原有的display属性，这就得先记下来原有的display属性到底是block还是inline还是别的值。
// jQuery直接提供show()和hide()方法

var a = $('a[target=_blank]');
a.hide(); // 隐藏
a.show(); // 显示

获取DOM信息
// 浏览器可视窗口大小:
$(window).width(); // 800
$(window).height(); // 600

// HTML文档大小:
$(document).width(); // 800
$(document).height(); // 3500

// 某个div的大小:var div = $('#test-div');
div.width(); // 600
div.height(); // 300
div.width(400); // 设置CSS属性 width: 400px，是否生效要看CSS是否有效
div.height('200px'); // 设置CSS属性 height: 200px，是否生效要看CSS是否有效

// attr()和removeAttr()方法用于操作DOM节点的属性：
// <div name="Test" start="1">...</div>

var div = $('#test-div');
div.attr('data'); // undefined, 属性不存在
div.attr('name'); // 'Test'
div.attr('name', 'Hello'); // div的name属性变为'Hello'
div.removeAttr('name'); // 删除name属性
div.attr('name'); // undefined
// prop()方法和attr()类似，或者用is判断。

表单
对于表单元素，jQuery对象统一提供val()方法获取和设置对应的value属性，统一了各种输入框的取值和赋值的问题。
修改DOM结构
添加DOM
append()把DOM添加到最后，
prepend()则把DOM添加到最前。
after()把新节点插入到指定位置。
删除DOM
拿到jQuery对象后直接调用remove()方法就可以了。
事件
因为JavaScript在浏览器中以单线程模式运行，页面加载后，一旦页面上所有的JavaScript代码被执行完后，就只能依赖触发事件来执行JavaScript代码。
浏览器在接收到用户的鼠标或键盘输入后，会自动在对应的DOM节点上触发相应的事件。如果该节点已经绑定了对应的JavaScript处理函数，该函数就会自动调用。
由于不同的浏览器绑定事件的代码都不太一样，所以用jQuery来写代码，就屏蔽了不同浏览器的差异，我们总是编写相同的代码。
鼠标:
click: 鼠标单击时触发；
dblclick：鼠标双击时触发；
mouseenter：鼠标进入时触发；
mouseleave：鼠标移出时触发；
mousemove：鼠标在DOM内部移动时触发；
hover：鼠标进入和退出时触发两个函数，相当于mouseenter加上mouseleave。
键盘：
键盘事件仅作用在当前焦点的DOM上，通常是<input>和<textarea>。
keydown：键盘按下时触发；
keyup：键盘松开时触发；
keypress：按一次键后触发。
其他：
focus：当DOM获得焦点时触发；
blur：当DOM失去焦点时触发；
change：当<input>、<select>或<textarea>的内容改变时触发；
submit：当<form>提交时触发；
ready：当页面被载入并且DOM树完成初始化后触发。（仅作用于document对象）
所有事件都会传入Event对象作为参数，可以从Event对象上获取到更多的信息
$(function () {
    $('#testMouseMoveDiv').mousemove(function (e) {
        $('#testMouseMoveSpan').text('pageX = ' + e.pageX + ', pageY = ' + e.pageY);
    });
});

绑定事件
/* HTML:
 *
 * <a  >点我试试</a>
 *
 */

// 获取超链接的jQuery对象:
var a = $('#test-link');
a.on('click', function () {
    alert('Hello!');
});

on()方法用来绑定一个事件，需要传来事件名和对应的处理函数。
直接调用click()方法。
a.click(function () {
    alert('Hello!');
});

取消绑定
通过off('click', function)实现。
function hello() {
    alert('hello!');
}

a.click(hello); // 绑定事件

// 10秒钟后解除绑定:
setTimeout(function () {
    a.off('click', hello);
}, 10000);

触发条件
事件的触发总是由用户操作引发的。
var input = $('#test-input');
input.change(function () {
    console.log('changed...');
});

当用户在文本框中输入时，就会触发change事件。但是，如果用JavaScript代码去改动文本框的值，将不会触发change事件：
var input = $('#test-input');
input.val('change it!'); // 无法触发change事件

希望用代码触发change事件，可以直接调用无参数的change()方法来触发该事件：
var input = $('#test-input');
input.val('change it!');
input.change(); // 触发change事件 
//input.change()相当于input.trigger('change')，它是trigger()方法的简写。

在浏览器中，有些JavaScript代码只有在用户触发下才能执行，例如，window.open()函数：
动画
用JavaScript实现动画，原理非常简单：我们只需要以固定的时间间隔（例如，0.1秒），每次把DOM元素的CSS样式修改一点（例如，高宽各增加10%），看起来就像动画了。
但是要用JavaScript手动实现动画效果，需要编写非常复杂的代码。如果想要把动画效果用函数封装起来便于复用，那考虑的事情就更多了。
使用jQuery实现动画，代码已经简单得不能再简化了：只需要一行代码！
show / hide
直接以无参数形式调用show()和hide()，会显示和隐藏DOM元素。
只要传递一个时间参数进去，就变成了动画。
var div = $('#test-show-hide');
div.hide(3000); // 在3秒钟内逐渐消失, 时间以毫秒为单位

// toggle()方法则根据当前状态决定是show()还是hide()。如果当前是显示则用 hide()

slideUp / slideDown
show()和hide()是从左上角逐渐展开或收缩的，而slideUp()和slideDown()则是在垂直方向逐渐展开或收缩的。
slideToggle()则根据元素是否可见来决定下一步动作
fadeIn / fadeOut
fadeIn()和fadeOut()的动画效果是淡入淡出，也就是通过不断设置DOM元素的opacity属性来实现，而fadeToggle()则根据元素是否可见来决定下一步动作。
自定义
animate()，它可以实现任意动画效果，我们需要传入的参数就是DOM元素最终的CSS状态和时间，jQuery在时间段内不断调整CSS直到达到我们设定的值。
animate()还可以再传入一个函数，当动画结束时，该函数将被调用：
串行动画
通过delay()方法还可以实现暂停
AJAX
jQuery 对象绑定了一个 ajax() 对象，可以处理 Ajax 操作。
ajax(url, settings)函数需要接收一个URL和一个可选的settings对象。当然一般也不需要在直接操作这个函数，jQuery 对不同的请求方法封装了不同的函数方便使用。
get()
var jqxhr = $.get('/path/to/resource', {
   name: 'Bob Lee',
   check: 1
});

post
post()和get()类似，但是传入的第二个参数默认被序列化为application/x-www-form-urlencoded：
var jqxhr = $.post('/path/to/resource', {
   name: 'Bob Lee',
   check: 1
});

getJSON
由于JSON用得越来越普遍，所以jQuery也提供了getJSON()方法来快速通过GET获取一个JSON对象：
var jqxhr = $.getJSON('/path/to/resource', {
    name: 'Bob Lee',
    check: 1
}).done(function (data) {
    // data已经被解析为JSON对象了
});

jQuery的AJAX完全封装的是JavaScript的AJAX操作，所以它的安全限制和前面讲的用JavaScript写AJAX完全一样。
错误处理
try ... catch ... finally
try {
    r1 = s.length; // 此处应产生错误
    r2 = 100; // 该语句不会执行
} catch (e) {
    console.log('出错了：' + e);
} finally {
    console.log('finally');
}
console.log('r1 = ' + r1); // r1应为undefined
console.log('r2 = ' + r2); // r2应为undefined

错误类型
JavaScript有一个标准的Error对象表示错误，还有从Error派生的TypeError、ReferenceError等错误对象
抛出错误
try {
    s = prompt('请输入一个数字');
    n = parseInt(s);
    if (isNaN(n)) {
        throw new Error('输入错误');  //抛出错误
    }
    // 计算平方:
    r = n * n;
    console.log(n + ' * ' + n + ' = ' + r);
} catch (e) {
    console.log('出错了：' + e);
}


undersore
underscore 是一个第三方库，提供了统一的函数式编程接口。
和 jQuery 类似，underscore 会把自身绑定到一个唯一的全局变量“_”中。

Node.js
基于JavaScript语言和V8引擎的开源Web服务器项目，命名为Node.js。
npm其实是Node.js的包管理工具（package manager）。
Node第一次把JavaScript带入到后端服务器开发。
web 框架 kor、Vue、Angular、React。
MySQL Node.js驱动程序是开源的mysql。
Node的ORM框架Sequelize。
mocha是JavaScript的一种单元测试框架。
WebSocket是HTML5新增的协议，它的目的是在浏览器和服务器之间建立一个不受限的双向通信的通道，
REST就是一种设计API的模式。最常用的数据格式是JSON。由于JSON能直接被JavaScript读取，所以，以JSON格式编写的REST风格的API具有简单、易读、易用的特点。