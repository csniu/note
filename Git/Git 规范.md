本文介绍 [Angular](https://docs.google.com/document/d/1QrDFcIiPjSLDn3EL15IJygNPiHORgU1_OOAqWjiDU5Y/edit#heading=h.greljkmo14y0) 规范.

Commit message 分为三个部分：Header，Body 和 Footer。
- Body、Footer 可省略。
- 任何一行不超过 72 个字符。

格式如下：
```
<type>(<scope>): <subject>
// 空一行
<body>
// 空一行
<footer>
```

#### Header
只有一行，包括三个字段：type（必需）、scope（可选）和 subject（必需）。
- type：commit 的类别。
    - feat：新功能（feature）
    - fix：修补bug
    - docs：文档（documentation）
    - style： 格式（不影响代码运行的变动）
    - refactor：重构（即不是新增功能，也不是修改bug的代码变动）
    - test：增加测试
    - chore：构建过程或辅助工具的变动
- scope：commit 影响的范围。
    - 视项目不同而不同，比如数据层、控制层、视图层等等。
- subject：主题

#### Body
是对本次 commit 的详细描述，可以分成多行。

说明代码变动的动机，以及与以前行为的对比。


#### Footer
只要一下情况才应该有：
- 不兼容变动，以BREAKING CHANGE开头，后面是对变动的描述、以及变动理由和迁移方法。
- 关闭 Issue。`Closes #234`

#### Revert
一种特殊情况，用于撤销以前的 commit。
- 必须以revert:开头，后面跟着被撤销 Commit 的 Header。
- Body 必须写成 `必须写成This reverts commit &lt;hash>.`,其中的hash是被撤销 commit 的 SHA 标识符。
*****
#### Commitizen
Commitizen 是一个撰写合格
[Commitizen](https://github.com/commitizen/cz-cli)是一个撰写合格 Commit message 的工具。

\# Commitizen 是基于 Node 的，所有需要先安装 Node 环境。对于非 Node 项目需要先执行`npm init --yes`进行初始化。 

##### 安装
```
npm install -g commitizen
```
- `-g`: 全局安装

##### 使用 Angular 的 Commit message 规范
- 项目安装
```
commitizen init cz-conventional-changelog --save --save-exact
```

全局安装：
```
npm install -g commitizen cz-conventional-changelog
echo '{ "path": "cz-conventional-changelog" }' > ~/.czrc
```

定制自己的规范：

可以通过指定 Adapter [cz-customizable](https://github.com/leoforfree/cz-customizable) 指定一套符合自己团队的规范.


##### 提交修改
使用 `git cz` 代替 `git commit`操作。

\# 但是`git commit`以然可以正常使用。

##### CHANGELOG 文件
安装插件：
```
conventional-changelog
conventional-changelog-cli
```
生成 CHANGELOG.md 文件：
```
conventional-changelog -p angular -i CHANGELOG.md -s
```

\# [standard-version](https://github.com/conventional-changelog/standard-version)
> 

##### 强制验证提交规范
正常情况下，使用`git commit`还是可以正常提交的。可以通过[Commitlint](https://conventional-changelog.github.io/commitlint/)强制验证提交是否符合规范，如果不符合则直接拒绝提交。

安装:
```
npm i -D @commitlint/config-conventional @commitlint/cli
```
同时需要在项目目录下创建配置文件 .commitlintrc.js, 写入:
```
module.exports = {
  extends: [
    ''@commitlint/config-conventional''
  ],
  rules: {
  }
};
```

结合 `git hook`：
```
npm i husky@next
```
package.json 中添加:
```
"husky": {
    "hooks": {
      "commit-msg": "commitlint -e $GIT_PARAMS"
    }
  },

```

参考： 
- 阮一峰-[Commit message 和 Change log 编写指南](https://www.ruanyifeng.com/blog/2016/01/commit_message_change_log.html)
- [优雅的提交你的 Git Commit Message](https://juejin.im/post/6844903606815064077)