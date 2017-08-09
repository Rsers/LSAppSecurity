//
//  LZDeviceDetection.h
//  LZAppReinforce-01-Jailbroken
//
//  Created by 邢雷镇 on 2017/8/8.
//  Copyright © 2017年 Rayz. All rights reserved.
//

#ifndef LZDeviceDetection_h
#define LZDeviceDetection_h

/*- - - - - - - - - - - - jailbroken check - - - - - - - - - - - - */
#import <unistd.h>
#import <stdlib.h>
#import <sys/stat.h>
#import <dlfcn.h>
#import <mach-o/dyld.h>

#import <UIKit/UIKit.h>

//static inline : 防止静态分析

//sandBox integrity check
static int check_jailbroken_sandbox () __attribute__((always_inline));
int check_jailbroken_sandbox () {
    int result = fork();
    if (!result) {
        exit(0);
    }
    if (result>=0) {
        return 1;
    }
    return 0;
}

//check a tweak
static int check_jailbroken_tweak () __attribute__((always_inline));
int check_jailbroken_tweak () {
    //链接：http://www.jianshu.com/p/bff550b3ead4
    uint8_t lmb[] = {'S', 'u', 'b', 's', 't', 'r', 'a', 't', 'e', '/', 'D', 'y', 'n', 'a', 'm', 'i', 'c', 0, };
    NSString *dir = [NSString stringWithFormat:@"/%@/%@%s%@", @"Library", @"Mobile", lmb, @"Libraries"];
    NSArray *dirFiles = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:dir error:nil];
    NSArray *plistFiles = [dirFiles filteredArrayUsingPredicate:
                           [NSPredicate predicateWithFormat:
                            [NSString stringWithFormat:@"%@ %@%@ '.%@%@'",@"self", @"EN", @"DSWITH", @"pli", @"st"]]];
    int cnt = 0;
    for (NSString *file in plistFiles) {
        NSString *filePath = [dir stringByAppendingPathComponent:file];
        NSString *fileContent = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil];
        if (fileContent && [fileContent rangeOfString:[[NSBundle mainBundle] bundleIdentifier]].location != NSNotFound) {
            cnt ++;
        }
    }
    // 返回有针对本 app 的 tweak 数量，为 0 说明没有
    return cnt;
}

//check plugin
static int check_jailbroken_cydia () __attribute__((always_inline));
int check_jailbroken_cydia () {
    struct stat s;
    int is_jailbroken =
    stat("Library/MobileSubstrate/MobileSubstrate.dylib", &s) == 0 || stat("/Applications/Cydia.app", &s) == 0 || stat("/var/lib/cydia/", &s) == 0 || stat("/var/cache/apt", &s) == 0;
    return is_jailbroken;
}

//check dylib
static int check_jailbroken_dylib () __attribute__((always_inline));
int check_jailbroken_dylib () {
    int ret;
    Dl_info dylib_info;
    int (*func_stat)(const char *,struct stat *) = stat;
    int is_jailbroken = 0;
    if ((ret = dladdr(func_stat, &dylib_info))) {
        if (strcmp(dylib_info.dli_fname, "/usr/lib/system/libsystem_kernel.dylib")) {
            is_jailbroken = 1;
        }
    }
    return is_jailbroken;
}

//check dyld
static int check_jailbroken_dyld () __attribute__((always_inline));
int check_jailbroken_dyld () {
    char *env = getenv("DYLD_INSERT_LIBRARIES");
    int is_jailbroken = 0;//no
    if (env != NULL || env != nil) {
        is_jailbroken = 1;//yes
    }
    return is_jailbroken;
}

static inline __attribute__((always_inline)) void found_jailbroken_device (){
    exit(-1);
}


#endif /* LZDeviceDetection_h */
