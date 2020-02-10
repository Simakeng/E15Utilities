using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;

using MessageBox = System.Windows.Forms.MessageBox;

namespace ProjectWizard
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private static string solutionDir = "";

        private static string GetResourceContent(string name)
        {
            Stream sm = Assembly.GetExecutingAssembly().GetManifestResourceStream(name);
            StreamReader sr = new StreamReader(sm);
            return sr.ReadToEnd();
        }

        public MainWindow()
        {
            InitializeComponent();
        }

        private string GetProjectType()
        {
            switch ((cbProjType.SelectedItem as ComboBoxItem).Content)
            {
                case "静态库": return @"StaticLibrary";
                case "动态库": return @"DynamicLibrary";
                case "控制台程序": return @"Application";
                case "窗口程序": return @"Application";
                case "单元测试": return @"Application";
                case "代码工具": return @"Application";
            }
            return "";
        }

        private string GetSubSystem()
        {
            switch ((cbProjType.SelectedItem as ComboBoxItem).Content)
            {
                case "静态库": return @"Console";
                case "动态库": return @"Console";
                case "控制台程序": return @"Console";
                case "窗口程序": return @"Windows";
                case "单元测试": return @"Console";
                case "代码工具": return @"Console";
            }
            return "";
        }

        private string GetProjPath()
        {
            switch ((cbProjType.SelectedItem as ComboBoxItem).Content)
            {
                case "静态库": return @"Utilities\";
                case "动态库": return @"Utilities\";
                case "控制台程序": return @"";
                case "窗口程序": return @"";
                case "单元测试": return @"Tests\";
                case "代码工具": return @"tools\";
            }
            return "";
        }

        private bool pathEdited = false;

        private void OnStartup(object sender, RoutedEventArgs e)
        {
            solutionDir = Process.GetCurrentProcess().MainModule.FileName;
            var start = solutionDir.LastIndexOf("tools");
            if (start != -1)
            {
                solutionDir = solutionDir.Remove(start);
            }

            tbProjPath.Text = solutionDir + GetProjPath();
        }

        private void OnUpdatePath(object sender, SelectionChangedEventArgs e)
        {
            if (!pathEdited)
                tbProjPath.Text = solutionDir + GetProjPath();
        }

        private void OnPathUpdated(object sender, TextChangedEventArgs e)
        {
            pathEdited = true;
        }

        private void OnBrowsePath(object sender, RoutedEventArgs e)
        {
            var fbd = new FolderBrowserDialog();
            fbd.ShowNewFolderButton = true;
            fbd.Description = "选择要生成的位置";
            if (fbd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                tbProjPath.Text = fbd.SelectedPath;
            }
        }

        private static bool ContainsIllegalChars(string s)
        {
            return
                s.IndexOf(':') != -1 &&
                s.IndexOf('/') != -1 &&
                s.IndexOf('\\') != -1 &&
                s.IndexOf('*') != -1 &&
                s.IndexOf('?') != -1 &&
                s.IndexOf('<') != -1 &&
                s.IndexOf('>') != -1 &&
                s.IndexOf('|') != -1;
        }

        private void OnGenerateProject(object sender, RoutedEventArgs e)
        {
            try
            {
                var projectName = tbProjName.Text.Trim();
                var projectPath = tbProjPath.Text.Trim();
                if (!Directory.Exists(projectPath))
                {
                    if (MessageBox.Show("所设置的路径不存在，要创建吗?", "错误", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == System.Windows.Forms.DialogResult.Yes)
                        Directory.CreateDirectory(projectPath);
                    else
                        return;
                }
                var targetPath = Path.Combine(projectPath, projectName);

                if (ContainsIllegalChars(projectName))
                { MessageBox.Show("项目名称不能包括如下字符:\n: / \\ * ? < > |", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error); return; }

                if (!Directory.Exists(targetPath))
                    Directory.CreateDirectory(targetPath);
                else
                {
                    if (Directory.GetFiles(targetPath).Length != 0)
                    {
                        if (MessageBox.Show("生成路径不为空，该操作将会清空生成路径，继续吗？", "错误", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == System.Windows.Forms.DialogResult.Yes)
                        {
                            DirectoryInfo info = new DirectoryInfo(targetPath);
                            foreach (var file in info.GetFiles()) file.Delete();
                            foreach (var dir in info.GetDirectories()) dir.Delete();
                        }
                        else return;
                    }
                }

                var filters = GetResourceContent("ProjectWizard.Project.Template.Project.Template.filters");
                File.WriteAllText(Path.Combine(targetPath, $"{projectName}.filters"), filters);

                var user = GetResourceContent("ProjectWizard.Project.Template.Project.Template.user");
                File.WriteAllText(Path.Combine(targetPath, $"{projectName}.user"), filters);

                var vcproj_user = GetResourceContent("ProjectWizard.Project.Template.Project.Template.vcxproj.user");
                File.WriteAllText(Path.Combine(targetPath, $"{projectName}.vcxproj.user"), vcproj_user);

                var vcproj = GetResourceContent("ProjectWizard.Project.Template.Project.Template.vcxproj");

                vcproj = vcproj.Replace("|#projname#|", projectName);

                vcproj = vcproj.Replace("|#projguid#|", $"{Guid.NewGuid()}");

                vcproj = vcproj.Replace("|#projnamespace#|", projectName.Replace(".", ""));

                vcproj = vcproj.Replace("|#projsubsys#|", GetSubSystem());

                vcproj = vcproj.Replace("|#projtype#|", GetProjectType());

                File.WriteAllText(Path.Combine(targetPath, $"{projectName}.vcxproj"), vcproj);

                MessageBox.Show("生成完成", "成功", MessageBoxButtons.OK);
            }
            catch (Exception ex)
            {
                MessageBox.Show("生成失败\n" + ex.Message, "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}