# clion

## 开启Clangd

在CLion中配置Clangd作为索引的引擎，需要按照以下步骤操作：
1. 在CLion中，点击顶部的File菜单，选择Settings。
2. 在打开的设置窗口左侧的面板中，选择Languages & Frameworks -> C/C++ -> Clangd。
3. 在右侧的面板中，你可以配置Clangd的相关选项。
- Clangd executable: 这里你需要指定你的clangd执行文件的路径。如果你在系统环境变量中已经设置了clangd的路径，CLion会自动检测并填写这个路径。
- Clangd arguments: 在这个输入框中，你可以添加传递给clangd的参数，用空格分隔。这个输入框可以留空。
- Completion and navigation are powered by Clangd: 这是一个开关选项，你需要打开它，才能使Clangd作为代码提示和导航的引擎。
记得在所有设置完成后点击Apply和OK按钮保存你的配置。
这样，你就成功地配置了Clangd作为CLion的索引引擎。你会发现代码提示和导航的速度变快了，因为Clangd是一个性能很高的语言服务器，专门为这些功能优化的。

## 使用Clang-Format

在CLion中使用Clang-Format作为代码格式化工具，你可以按照以下步骤操作：
1. 确保你的系统中已经安装了Clang-Format。在macOS中，你可以使用Homebrew (brew install clang-format) 安装。
2. 在CLion中，选择 Preferences 或者 Settings，然后在弹出的对话框中选择 Editor > Code Style。
3. 在Code Style选项中，你可以看到右边有一个Enable ClangFormat的选项，选中它就可以启用ClangFormat功能。
4. 默认情况下，CLion会使用Bundled（内置）的Clang-Format，你也可以选择自己系统中安装的Clang-Format版本，只需在Clang-Format binary中输入对应的路径即可。
启用ClangFormat后，每次保存文件或者手动格式化代码时（Code > Reformat Code），CLion就会使用Clang-Format按照你设置的样式规则来自动格式化代码。如果你已经在工程目录中包含了.clang-format文件作为你的样式配置，CLion会自动使用这个配置。没有的话，CLion会使用默认的LLVM样式。
注意，Clang-Format只适用于C/C++/ObjC的代码格式化，在其他语言上可能无法使用。