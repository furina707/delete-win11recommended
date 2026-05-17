#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>

// 函数声明
void RestoreRecentItemsVisibility(void);
void EnableStartTrackDocs(void);
void EnableShowRecentList(void);

int main()
{
    // 设置控制台编码为GBK
    SetConsoleOutputCP(936);
    SetConsoleCP(936);
    
    printf("====================================\n");
    printf("       恢复显示最近项目工具          \n");
    printf("====================================\n\n");
    
    RestoreRecentItemsVisibility();
    EnableStartTrackDocs();
    EnableShowRecentList();
    
    printf("\n====================================\n");
    printf("操作完成！按任意键退出...\n");
    printf("====================================\n");
    getchar();
    
    return 0;
}

void RestoreRecentItemsVisibility(void)
{
    char path[MAX_PATH];
    char cmd[MAX_PATH + 100];
    
    // 获取APPDATA路径
    if (GetEnvironmentVariableA("APPDATA", path, MAX_PATH) > 0)
    {
        strcat(path, "\\Microsoft\\Windows\\Recent Items");
        printf("最近项目文件夹: %s\n", path);
        printf("[OK] 文件夹路径已确认\n");
        printf("[提示] Recent Items文件夹不需要\"开启\"，它是系统自动创建的\n");
        printf("       当你打开文件时，系统会自动在这里创建快捷方式\n");
    }
    else
    {
        printf("[失败] 无法获取APPDATA路径\n");
    }
}

void EnableStartTrackDocs(void)
{
    HKEY hKey;
    DWORD dwValue = 1;  // 1 = 开启显示
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
            printf("[OK] 已开启\"显示最近打开的项目\"\n");
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

void EnableShowRecentList(void)
{
    HKEY hKey;
    DWORD dwValue = 1;  // 1 = 开启显示
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
            printf("[OK] 已开启\"显示最近添加的应用\"\n");
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