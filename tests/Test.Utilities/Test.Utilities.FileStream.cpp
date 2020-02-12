/**
 @file
 @brief 对 Utilities::FileStream 进行单元测试

 这个文件里面是通过几组函数对 Utilities::FileStream 进行功能上的单元测试

 @author 司马坑
 @date 2020/2/12
*/

#include <gtest/gtest.h>

#include <Utilities.Stream.h>
#pragma comment(lib,"Utilities.Stream.lib")

using namespace std;
using namespace Utilities;

TEST(Utilities_FileStream,CreatStream) 
{
    Stream s;
}
