#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_Model_Data(char *text)
{
    // 2.加载模型识别
    FILE *fp = popen("curl -s -i -X POST -H \"Content-Type: audio/wav;rate=16000\" \"
    {
        printf("加载模型失败\n");
        return -1;
    }   
    else
    {
        printf("加载模型成功\n");
    }

    // 读取模型的数据
    while (1)
    {
        char buf[4096] = {0};
        char *ret = fgets(buf, 4096, fp);
        if (ret == NULL)
        {
            break;
        }

        // 数据过滤
        if (strstr(buf, "result")) // 判断是否为结果
        {
            // printf("%s\n", buf);

            //{"corpus_no":"76459832","err_msg":"success.","err_no":0,"result":["你好你好。"],"sn":"8483219190"}
            char *p = strstr(buf, "[");

            // 偏移一个位置找后面的 ]
            char *end = strstr(p + 1, "]");

            // 添加结束符号 ,end - 1 指向 "
            *(end - 1) = '\0';

            // 输出处理后的数据
            printf("%s\n", p + 2);

            strcpy(text, p + 2); // 拷贝数据到形参中
        }
    }

    printf("识别结果\n");

    pclose(fp);
}

int main()
{

    printf("开始录制....\n");

    // 1.录制音频数据
    system("arecord -d 5 -f S16_LE -r 16000 -c 1 test.wav");

    printf("录制完毕开始识别...\n");

    char text[1024] = {0};
    get_Model_Data(text);

    printf("识别结果:%s\n", text);
}