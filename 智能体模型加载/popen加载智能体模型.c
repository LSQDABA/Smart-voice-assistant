#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 获取大模型的应答数据
// 参数一:用户的问题
// 参数二:模型返回的答案
int get_doubao_model(char *question, char *answer)
{
    char cmd[4096] = {0};
    
    // 对用户输入进行 JSON 转义（处理双引号和反斜杠）
    char escaped_input[2048] = {0};
    char *src = question;
    char *dst = escaped_input;
    
    while (*src && (dst - escaped_input) < (sizeof(escaped_input) - 2)) {
        if (*src == '"' || *src == '\\') {
            *dst++ = '\\';  // 添加转义符
        }
        *dst++ = *src++;
    }
    *dst = '\0';
    
    // 2.格式化拼接完整curl指令，%s填充用户输入
    snprintf(cmd, sizeof(cmd),
        "curl -s -X POST 'https://api.coze.cn/v3/chat?' "
        "-H \"Authorization: Bearer $BEARER_TOKEN\" "
        "-H \"Content-Type: application/json\" "
        "-d '{\"bot_id\": \"       \", \"user_id\": \"123456789\", \"stream\": true, "
        "\"additional_messages\": [{\"content\": \"%s\", \"content_type\": \"text\", "
        "\"role\": \"user\", \"type\": \"question\"}], \"parameters\": {}}'",
        escaped_input);

    // 打印最终拼接好的cmd，调试查看
    // printf("\n生成的curl命令：\n%s\n", cmd);

    // 加载模型获取应答数据
    FILE *fp = popen(cmd, "r");
    if (fp == NULL)
    {
        printf("加载模型失败\n");
        return -1;
    }

    while (1)
    {
        char buf[4096] = {0};

        char *ret = fgets(buf, 4096, fp);
        if (ret == NULL)
        {
            break;
        }

        // 处理模型返回的数据包体
        if (strstr(buf, "created_at") != NULL && strstr(buf, "answer") != NULL && !strstr(buf, "verbose"))
        {
            // printf("%s\n", buf);

            /*
            data:{"id":"7648172515429285924","conversation_id":"7648172511134122003","bot_id":"7648101662381064218",
                "role":"assistant","type":"answer",
                "content":"你好呀！你真的很有礼貌，主动打招呼的样子超亲切呀～我想先了解一下你，你觉得自己最近做过最棒的事情是什么呢？",
                "content_type":"text", "chat_id":"7648172511134171155",
                "section_id":"7648172511134122003","created_at":1780728931,"time_cost":{"total_duration_ms":2156}}
            */

            char *p = strstr(buf, "content") + 10; // 指向第一个汉字

            char *end = strstr(p, "\""); // 指向 最后一个 "

            *end = '\0'; // 添加结束符号

            strcpy(answer, p);
            break; // 找到答案后退出循环
        }
    }

    // 关闭模型
    pclose(fp);
    return 0;
}

int main(void)
{
    char user_input[1024] = {0}; // 用户输入内容缓冲区

    // 1.接收用户输入content文本
    printf("请输入发送给AI的消息：");
    fgets(user_input, sizeof(user_input), stdin);  // 使用 fgets 代替 scanf 以支持空格
    user_input[strcspn(user_input, "\n")] = 0;     // 移除末尾换行符

    char text[1024] = {0};

    get_doubao_model(user_input, text); // 调用大模型获取答案

    printf("AI回答: %s\n", text);

    return 0;
}