using System;
using System.Diagnostics;
using System.IO;
using System.Threading;

namespace AutoDoxygen
{
    internal class Program
    {
        /// <summary>
        /// 获取当前储存库路径
        /// </summary>
        /// <returns></returns>
        private static string GetRepoPath()
        {
            if (repoPath != null)
                return repoPath;
            var fileName = Process.GetCurrentProcess().MainModule.FileName;
            var start = fileName.IndexOf("GameEngine");
            if (start != -1)
                start = start + "GameEngine".Length;
            else
                throw new Exception("Failed on GetRepoPath");
            return fileName.Remove(start);
        }

        private enum GitStatus
        {
            Success,
            Fail
        };

        /// <summary>
        /// 调用Git fetch 命令并检测结果
        /// </summary>
        /// <returns></returns>
        private static GitStatus GetStatus()
        {
            try
            {
                var path = GetRepoPath();
                Process process = new Process();
                process.StartInfo.UseShellExecute = false;   // 是否使用外壳程序
                process.StartInfo.CreateNoWindow = true;   //是否在新窗口中启动该进程的值
                process.StartInfo.RedirectStandardInput = true;  // 重定向输入流
                process.StartInfo.RedirectStandardOutput = true;  //重定向输出流
                process.StartInfo.RedirectStandardError = true;  //重定向错误流
                process.StartInfo.FileName = "cmd.exe";
                process.Start();
                process.StandardInput.WriteLine($"cd {path}");
                process.StandardInput.WriteLine($"git fetch");
                process.StandardInput.WriteLine($"git fetch");
                process.StandardInput.WriteLine($"exit");
                var so = process.StandardOutput.ReadToEnd();
                var eo = process.StandardError.ReadToEnd();
                if (eo.Length != 0)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine($"[错误]{eo}");
                    Console.ResetColor();
                    return GitStatus.Fail;
                }
                return GitStatus.Success;
            }
            catch (Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"[错误]{ex.Message}");
                Console.ResetColor();
                return GitStatus.Fail;
            }
        }

        private static void GenerateDocs()
        {
            try
            {
                var path = GetRepoPath();
                Process process = new Process();
                process.StartInfo.UseShellExecute = false;   // 是否使用外壳程序
                process.StartInfo.CreateNoWindow = true;   //是否在新窗口中启动该进程的值
                process.StartInfo.RedirectStandardInput = true;  // 重定向输入流
                process.StartInfo.RedirectStandardOutput = true;  //重定向输出流
                process.StartInfo.RedirectStandardError = true;  //重定向错误流
                process.StartInfo.FileName = "cmd.exe";
                process.Start();
                process.StandardInput.WriteLine($"cd {path}");
                process.StandardInput.WriteLine($"doxygen Doxyfile");
                process.StandardInput.WriteLine($"exit");
                var so = process.StandardOutput.ReadToEnd();
                var eo = process.StandardError.ReadToEnd();
                if (eo.Length != 0)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine($"[错误]{eo}");
                    Console.ResetColor();
                    return;
                }
                Console.WriteLine("[消息]文档已经重新生成！");
            }
            catch (Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"[错误]{ex.Message}");
                Console.ResetColor();
            }
        }

        private static bool needUpdate = false;
        private static string repoPath = null;

        private static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                var temp = Path.Combine(Environment.GetEnvironmentVariable("TEMP"), "autogen.exe");
                File.Copy(Process.GetCurrentProcess().MainModule.FileName, temp, true);
                Process.Start(temp, GetRepoPath());
                return;
            }
            else
            {
                repoPath = args[0];
            }
            FileSystemWatcher sw = new FileSystemWatcher();
            sw.Path = GetRepoPath();
            sw.IncludeSubdirectories = true;
            sw.NotifyFilter = NotifyFilters.LastWrite;
            sw.Changed += new FileSystemEventHandler(OnChanged);

            sw.EnableRaisingEvents = true;
            Thread GitUpdateThread = new Thread(() =>
            {
                while (true)
                {
                    GetStatus();
                    Thread.Sleep(1000 * 60 * 1);
                }
            });
            GitUpdateThread.Start();
            Thread DoxygenWorkerThread = new Thread(() =>
            {
                while (true)
                {
                    Thread.Sleep(1000 * 60 * 1);
                    if (needUpdate)
                    {
                        needUpdate = false;
                        GenerateDocs();
                    }
                }
            });
            DoxygenWorkerThread.Start();
            while (true)
                Thread.Sleep(1);
        }

        public static void OnChanged(object source, FileSystemEventArgs e)
        {
            if (e.FullPath.Contains(Path.Combine(GetRepoPath(), "docs"))) // 忽略生成目录
                return;
            if (e.FullPath.Contains("\\.")) // 忽略隐藏文件变化
                return;

            needUpdate = true;
            Console.WriteLine($"{e.ChangeType}  {e.FullPath}");
        }
    }
}