# GitHub 一键上传工具使用说明

`github-upload.ps1` 会自动完成以下操作：

1. 克隆 GitHub 仓库的最新版本；
2. 将指定的本地代码复制到仓库指定目录；
3. 执行 `git add` 和 `git commit`；
4. 等待提交完成后执行 `git push`；
5. 上传成功后清理临时目录。

脚本不会使用 `--force`，因此不会强制覆盖远程提交历史。任一步发生错误，后续步骤都会停止。

## GUI 图形界面（推荐）

直接双击工具目录中的：

```text
start-github-upload-gui.cmd
```

打开窗口后：

1. `Local source directory`：输入每次要上传的本地代码目录，或点击 `Browse...` 选择；
2. `GitHub repository URL`：输入本次要上传的 GitHub 仓库地址；
3. `Destination path inside repository`：输入代码要放入仓库的哪个目录，例如 `example/V1.2`；上传到仓库根目录时填写 `.`；
4. `Commit message`：输入本次修改说明；
5. `Branch`：一般留空，脚本会使用仓库默认分支；只有明确需要 `master` 等指定分支时才填写；
6. 点击 `Upload`，等待状态显示 `Upload completed successfully`。

如果浏览器能打开 GitHub，但日志显示 `Connection was reset`，在 `HTTP proxy` 中填写代理软件提供的本地 HTTP 代理地址。你的电脑检测到正在运行 v2rayN，可在 v2rayN 设置中查看本地 HTTP 端口，常见格式为 `http://127.0.0.1:10809`，但应以软件实际显示为准。该代理只用于本次 GUI 上传，不会修改 Git 全局配置。

以上路径和仓库地址都不是固定配置，每次上传前可以输入不同内容。窗口底部会实时显示 Git 和上传进度。

上传期间状态栏显示橙色；上传成功后显示绿色 `SUCCESS`；上传失败后显示红色 `FAILED`。完成后 GUI 不会自动关闭，详细成功信息或失败原因会一直保留在底部日志区域，直到再次上传或手动关闭窗口。

上传过程中，底部日志会持续刷新，每一步会显示类似 `[1/6]`、`[2/6]` 的阶段信息；状态栏会同步显示最近一条正在执行的操作。如果停留在 `Fetching`、`Rebase` 或 `Pushing`，通常表示正在等待网络或 GitHub 响应。

如果失败，日志会额外记录 PowerShell 版本、Git 版本、实际使用的仓库地址、启动参数和退出码。失败日志文件会保留在系统临时目录，GUI 底部会显示具体路径；把该日志内容发出来，就能判断是未登录、没有仓库权限、网络失败，还是本地 Git 冲突。

### 本地 `.git` 和第二次增量上传

当 `Destination path inside repository` 填写 `.` 时，本地代码目录会作为完整 Git 仓库使用：

- 第一次上传时，会在所选的本地代码目录中创建隐藏的 `.git` 文件夹；
- 第二次及以后上传时，会复用这个 `.git`，自动检测所有新增、修改和删除的文件；
- 上传前会获取 GitHub 上的最新提交，避免覆盖其他人刚上传的版本；
- `.git` 默认是 Windows 隐藏目录，需要在资源管理器中打开“查看 → 隐藏的项目”才能看到。

如果第二次增加文件后 GitHub 没变化，请先查看绿色或红色状态，以及底部日志。正常新增文件时，日志中会出现 `Creating a local commit` 和 `Pushing ... commit(s)`。

新创建且完全为空的 GitHub 仓库没有 `main` 或 `master` 分支。工具会识别这种情况，跳过空仓库的首次 `fetch`，在本地创建第一条提交后再建立 `origin` 联系并推送。

当目标路径填写 `example/V1.2` 等仓库子目录时，工具仍会使用临时克隆模式，因为本地文件夹只是远程仓库的一部分；这种模式不会在本地创建 `.git`。如果希望本地永久保留 `.git`，请选择完整的本地仓库目录，并将目标路径填写为 `.`。

`Mirror mode` 默认不要勾选。勾选后，GitHub 目标目录中存在、但本地不存在的文件也会被删除，GUI 会再次要求确认。

如果仓库需要登录，第一次上传时建议先按照“首次准备”完成 HTTPS 或 SSH 身份验证。由于 GUI 后台执行 Git，无法在日志框中交互输入用户名或密码。

## 一、首次准备

### 1. 安装 Git

在 PowerShell 中执行：

```powershell
git --version
```

能显示版本号即可。如果提示找不到命令，请先从 <https://git-scm.com/download/win> 安装 Git，然后重新打开 PowerShell。

### 2. 设置 Git 提交身份

首次使用 Git 时执行一次：

```powershell
git config --global user.name "你的名字"
git config --global user.email "你的GitHub邮箱"
```

### 3. 登录 GitHub

推荐使用以下任意一种方式：

- HTTPS 地址：首次推送时按 Git 提示登录 GitHub。GitHub 不接受账户密码，应使用浏览器授权或 Personal Access Token。
- SSH 地址：提前配置 SSH Key，然后使用 `git@github.com:组织名/仓库名.git`。

## 二、最常用的一键上传

打开 PowerShell，进入本工具所在目录：

```powershell
cd "C:\Users\14175\Desktop\github_upload"
```

将整个本地代码目录上传到仓库根目录：

```powershell
.\github-upload.ps1 `
  -SourcePath "F:\wiki\new_files" `
  -RepoUrl "https://github.com/Elecrow-RD/你的仓库.git" `
  -CommitMessage "更新示例代码"
```

脚本会自动识别远程仓库的默认分支，通常是 `main` 或 `master`。

## 三、上传到仓库中的指定目录

例如，将 `F:\wiki\new_files` 的内容放进仓库的 `example/V1.2`：

```powershell
.\github-upload.ps1 `
  -SourcePath "F:\wiki\new_files" `
  -RepoUrl "https://github.com/Elecrow-RD/你的仓库.git" `
  -TargetPath "example/V1.2" `
  -CommitMessage "同步 example/V1.2"
```

`-TargetPath` 不写时默认为仓库根目录 `.`。路径应使用仓库内的相对路径，不能写盘符或 `..`。

## 四、让远程目标目录与本地完全一致

默认模式只新增或覆盖文件，不会删除远程目标目录中已有的其他文件，符合原操作文档中的安全追加方式。

如果本地删除的文件也需要从 GitHub 删除，请额外添加 `-Mirror`：

```powershell
.\github-upload.ps1 `
  -SourcePath "F:\wiki\new_files" `
  -RepoUrl "https://github.com/Elecrow-RD/你的仓库.git" `
  -TargetPath "example/V1.2" `
  -CommitMessage "完整同步 example/V1.2" `
  -Mirror
```

注意：`-Mirror` 会删除远程目标目录中本地不存在的文件。首次使用建议先在测试仓库验证，特别不要在仓库根目录随意使用。

## 五、其他参数

指定分支，例如 `master`：

```powershell
-Branch "master"
```

保留临时工作目录以便检查：

```powershell
-KeepWorkDir
```

完整示例：

```powershell
.\github-upload.ps1 `
  -SourcePath "F:\wiki\new_files" `
  -RepoUrl "git@github.com:Elecrow-RD/你的仓库.git" `
  -TargetPath "example/V1.2" `
  -Branch "master" `
  -CommitMessage "更新 V1.2 示例" `
  -KeepWorkDir
```

## 六、常见问题

### PowerShell 禁止运行脚本

如果出现“系统上禁止运行脚本”，只为当前 PowerShell 窗口临时放行：

```powershell
Set-ExecutionPolicy -Scope Process Bypass
```

然后重新运行上传命令。关闭该窗口后设置自动失效。

### 提示没有权限或 Authentication failed

确认仓库地址正确，并确认当前 GitHub 账号对仓库有写入权限。HTTPS 方式重新完成浏览器授权，SSH 方式执行以下命令检查连接：

```powershell
ssh -T git@github.com
```

### 提示 push rejected

通常表示上传期间远程仓库刚好产生了新提交。脚本不会强推覆盖他人的内容；直接重新运行脚本，它会重新获取最新仓库再上传。

### 提示没有检测到代码变化

本地代码与 GitHub 对应目录内容相同，不需要再次提交，这是正常结果。

### 上传失败后如何检查

失败时脚本会显示并保留临时工作目录。进入提示的目录检查文件或手动执行 Git 命令即可；确认不再需要后可自行删除该临时目录。
