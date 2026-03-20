#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>

// 函数声明
void ClearRecentItems(void);
void DisableStartTrackDocs(void);
void DisableShowRecentList(void);

int main()
{
    // 设置控制台编码为GBK
    SetConsoleOutputCP(936);
    SetConsoleCP(936);
    
    printf("====================================\n");
    printf("         清理最近项目工具            \n");
    printf("====================================\n\n");
    
    ClearRecentItems();
    DisableStartTrackDocs();
    DisableShowRecentList();
    
    printf("\n====================================\n");
    printf("操作完成！按任意键退出...\n");
    printf("====================================\n");
    getchar();
    
    return 0;
}

void ClearRecentItems(void)
{
    char path[MAX_PATH];
    char cmd[MAX_PATH + 100];
    
    // 获取APPDATA路径
    if (GetEnvironmentVariableA("APPDATA", path, MAX_PATH) > 0)
    {
        strcat(path, "\\Microsoft\\Windows\\Recent Items");
        printf("最近项目文件夹: %s\n", path);
        
        // 构建删除命令
        sprintf(cmd, "cmd /c del /f /s /q \"%s\\*\" >nul 2>&1", path);
        system(cmd);
        
        printf("[OK] 已清空最近项目文件夹\n");
    }
    else
    {
        printf("[失败] 无法获取APPDATA路径\n");
    }
}

void DisableStartTrackDocs(void)
{
    HKEY hKey;
    DWORD dwValue = 0;
    LONG result;
    
    result = RegOpenKeyExA(HKEY_CURRENT_USER, 
                           "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",
                           0, KEY_SET_VALUE, &hKey);
    
    if (result == ERROR_SUCCESS)
    {
        result = RegSetValueExA(hKey, "Start_TrackDocs", 0, REG_DWORD, 
                               (const BYTE*)&dwValue, sizeof(dwValue));
        if (result == ERROR_SUCCESS)
        {
            printf("[OK] 已禁用\"显示最近打开的项目\"\n");
        }
        else
        {
            printf("[失败] 设置注册表值失败: %lu\n", result);
        }
        RegCloseKey(hKey);
    }
    else
    {
        printf("[失败] 无法打开注册表键: %lu\n", result);
    }
}

void DisableShowRecentList(void)
{
    HKEY hKey;
    DWORD dwValue = 0;
    LONG result;
    
    result = RegOpenKeyExA(HKEY_CURRENT_USER, 
                           "Software\\Microsoft\\Windows\\CurrentVersion\\Start",
                           0, KEY_SET_VALUE, &hKey);
    
    if (result == ERROR_SUCCESS)
    {
        result = RegSetValueExA(hKey, "ShowRecentList", 0, REG_DWORD, 
                               (const BYTE*)&dwValue, sizeof(dwValue));
        if (result == ERROR_SUCCESS)
        {
            printf("[OK] 已禁用\"显示最近添加的应用\"\n");
        }
        else
        {
            printf("[失败] 设置注册表值失败: %lu\n", result);
        }
        RegCloseKey(hKey);
    }
    else
    {
        printf("[失败] 无法打开注册表键: %lu\n", result);
    }
}