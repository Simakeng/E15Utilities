using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;

namespace Pack
{
    class Program
    {

        static void Main(string[] args)
        {
            //获取当前程序所在的文件路径
            string rootPath = "C:\\Users\\Administrator\\Desktop\\456";

            // 获取全部文件
            file e = new file();

            var allfile = e.GetFiles(rootPath);

            // 遍历全部文件里面的数据 类似与include"xxxxx.h"
            // #include\s*".+"
            for (int i = 0; i < allfile.Length; i++)
            {
                e.Search(allfile[i].FullName);
            }


            Console.WriteLine("OK.");
        }
    }

    class file
    {
        public List<Files> fileList = new List<Files>();

        public struct Files
        {
            public string FullName;
            public string Name;
            public bool flag;
        }

        /// <summary>
        /// 获得目录下所有文件或指定文件类型文件地址
        /// </summary>
        public Files[] GetFiles(string fullPath, bool isFullName = false)
        {
            try
            {
                fileList.Clear();

                DirectoryInfo dirs = new DirectoryInfo(fullPath);       //获得程序所在路径的目录对象
                DirectoryInfo[] dir = dirs.GetDirectories();            //获得目录下文件夹对象
                FileInfo[] file = dirs.GetFiles();                      //获得目录下文件对象
                int dircount = dir.Count();                             //获得文件夹对象数量
                int filecount = file.Count();                           //获得文件对象数量

                //循环文件夹
                for (int i = 0; i < dircount; i++)
                {
                    string pathNode = fullPath + "\\" + dir[i].Name;
                    GetMultiFile(pathNode, isFullName);
                }

                //循环文件
                for (int i = 0; i < filecount; i++)
                {
                    Files files = new Files { FullName = file[i].FullName, Name = file[i].Name, flag = false };

                    if (File(file[i].Name))
                    {
                        fileList.Add(files);
                    }
                }

                return fileList.ToArray();
            }
            catch (Exception ex)
            {

            }

            return null;
        }

        public bool GetMultiFile(string path, bool isFullName = false)
        {
            if (Directory.Exists(path) == false)
            { return false; }

            DirectoryInfo dirs = new DirectoryInfo(path);           //获得程序所在路径的目录对象
            DirectoryInfo[] dir = dirs.GetDirectories();            //获得目录下文件夹对象
            FileInfo[] file = dirs.GetFiles();                      //获得目录下文件对象
            int dircount = dir.Count();                             //获得文件夹对象数量
            int filecount = file.Count();                           //获得文件对象数量

            int sumcount = dircount + filecount;

            if (sumcount == 0)
            { return false; }

            //循环文件夹
            for (int i = 0; i < dircount; i++)
            {
                string pathNodeB = path + "\\" + dir[i].Name;
                GetMultiFile(pathNodeB, isFullName);
            }

            //循环文件
            for (int i = 0; i < filecount; i++)
            {
                Files files = new Files { FullName = file[i].FullName, Name = file[i].Name, flag = false };

                if (File(file[i].Name))
                {
                    fileList.Add(files);
                }
            }
            return true;
        }

        public bool File(string str)
        {
            string patternH = @"(.*)(\.h)$";

            return (Regex.IsMatch(str, patternH)) ? true : false;
        }

        public void Search(string str)
        {
            using (StreamReader sr = new StreamReader(str))
            {
                string line;
                var reg = "#include(\\s*\".+\")";
                Regex r = new Regex(reg);

                // 从文件读取并显示行，直到文件的末尾 
                while ((line = sr.ReadLine()) != null)
                {
                    Match m = r.Match(line); // 在字符串中匹配
                    if (m.Success)
                    {
                        for (int i = 0; i < fileList.Count; i++)
                        {
                            if ("#include \"" + fileList[i].Name + "\"" == m.Value)
                            {
                                fileList[i].flag = true;
                            }
                        }
                    }
                }
            }
        }
    }

}
