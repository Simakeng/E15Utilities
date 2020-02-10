using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace Pack
{
    internal class Program
    {
        private class HeaderFile
        {
            public string path;
            public List<string> subHeaders = new List<string>();
            public bool isTop = true;
            public bool loaded = false;

            public HeaderFile(string _path)
            {
                this.path = _path;
                string rootPath = Program.GetDirectory(this.path);
                string text = File.ReadAllText(path, Encoding.UTF8);
                foreach (Match headers in Regex.Matches(text, @"(?=#include).+"))
                {
                    var m = Regex.Match(headers.Value, "(?=\").*(?=\")");
                    if (m.Success)
                    {
                        string subHeader = Path.Combine(rootPath, m.Value.Substring(1));
                        subHeaders.Add(subHeader);
                    }
                }
            }
        }

        public static string GetDirectory(string fileName)
        {
            var start = fileName.LastIndexOf('\\');
            if (start != -1)
                return fileName.Substring(0, start);
            return fileName;
        }

        public static string GetIncludeFileName(string line)
        {
            foreach (Match headers in Regex.Matches(line, @"(?=#include).+"))
            {
                var m = Regex.Match(headers.Value, "(?=\").*(?=\")");
                if (m.Success)
                    return m.Value.Substring(1);
            }
            return null;
        }

        private static void AppendFileToStream(IEnumerable<Program.HeaderFile> headers, Dictionary<string, Program.HeaderFile> dict, StreamWriter sw)
        {
            foreach (var top in headers)
            {
                Stack<StreamReader> stack = new Stack<StreamReader>();
                stack.Push(new StreamReader(top.path, Encoding.UTF8));
                dict[top.path].loaded = true;
                string line = null;
                while (stack.Count != 0)
                {
                    var sr = stack.Pop();
                    while ((line = sr.ReadLine()) != null)
                    {
                        var header = GetIncludeFileName(line);
                        if (header != null)
                        {
                            var subheaderPath = Path.Combine(GetDirectory(top.path), header);
                            if (dict.ContainsKey(subheaderPath))
                            {
                                if (!dict[subheaderPath].loaded)
                                {
                                    dict[subheaderPath].loaded = true;
                                    stack.Push(sr);
                                    stack.Push(new StreamReader(subheaderPath, Encoding.UTF8));
                                }
                            }
                            else
                            {
                                Console.WriteLine($"[警告]检测到错误的 include 文件 {subheaderPath} 已保留");
                                sw.Write(line);
                            }
                        }
                        else sw.WriteLine(line);
                    }
                }
            }
        }

        private static void Main(string[] args)
        {
            try
            {
                string ProjType = args[0];
                string binaryFilePath = args[1];
                string ProjectPath = args[2];
                string OutIncludePath = args[3];
                string OutFileExt = args[5].Substring(1);
                string OutBinaryPath = args[4].Replace("{gen_type}", OutFileExt);

                if (ProjType == "application")
                {
                    Console.WriteLine("无需操作");
                }

                Directory.CreateDirectory(GetDirectory(OutIncludePath));
                Directory.CreateDirectory(GetDirectory(OutBinaryPath));
                File.Copy(binaryFilePath, OutBinaryPath, true);

                Dictionary<string, HeaderFile> dict = new Dictionary<string, Program.HeaderFile>();
                List<HeaderFile> list = new List<HeaderFile>();

                foreach (var file in Directory.GetFiles(ProjectPath))
                    if (file.EndsWith(".h"))
                    {
                        var header = new HeaderFile(file);
                        dict.Add(file, header);
                        list.Add(header);
                    }
                var HasTop = list.Aggregate(false, (bool all, HeaderFile next) => { return all || next.isTop; });
                if (!HasTop)
                    Console.WriteLine("[警告]可能出现了环形包含，建议检查代码");

                StreamWriter streamWriter = new StreamWriter(OutIncludePath, false, Encoding.UTF8);

                var topHeaders = list.Where(e => e.isTop);
                AppendFileToStream(topHeaders, dict, streamWriter);

                var headers = list.Where(e => !e.loaded);
                Program.AppendFileToStream(headers, dict, streamWriter);

                streamWriter.Close();
                Console.WriteLine("OK.");
            }
            catch (Exception ex)
            {
                Console.WriteLine("==================================================");
                Console.WriteLine(ex.Message);
                Console.WriteLine(ex.StackTrace);
                Console.WriteLine("==================================================");
                throw;
            }
        }
    }
}