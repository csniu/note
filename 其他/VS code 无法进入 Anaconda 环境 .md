## 环境
- VS code: 1.47.3 
- OS: Windows10 教育版，18363.959
- Anaconda: 1.9.7
- conda: 4.7.10
- Terminal: Git Bash (2.7.7)

## 问题
在 VS code 中使用 Git Bash 作为终端，无法进入 Anaconda 的虚拟环境。提示如下：
```
CommandNotFoundError: Your shell has not been properly configured to use 'conda activate'.
If using 'conda activate' from a batch script, change your
invocation to 'CALL conda.bat activate'.

To initialize your shell, run

    $ conda init <SHELL_NAME>

Currently supported shells are:
  - bash
  - cmd.exe
  - fish
  - tcsh
  - xonsh
  - zsh
  - powershell

See 'conda init --help' for more information and options.

IMPORTANT: You may need to close and restart your shell after running 'conda init'. 
```

## 解决
- 在 `.bashrc` 中加入
```
eval "$('你的conda.exe绝对路径' 'shell.bash' 'hook')"
```

- `.bash_profile` 添加
```
if [ -f ~/.bashrc ]; then
    source ~/.bashrc
fi
```

\# 参考[方法](https://stackoverflow.com/a/58520069) 