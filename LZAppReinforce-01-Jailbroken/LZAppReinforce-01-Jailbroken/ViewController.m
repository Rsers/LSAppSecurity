//
//  ViewController.m
//  LZAppReinforce-01-Jailbroken
//
//  Created by 邢雷镇 on 2017/8/8.
//  Copyright © 2017年 Rayz. All rights reserved.
//

#import "ViewController.h"

#define lz_SYSTEM_VERSION_THAN(__version,__condition) ([[UIDevice currentDevice].systemVersion floatValue] __condition __version)

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    [self lz_deviceJailBroken];
    
    
    
//    [[UIDevice currentDevice].systemVersion floatValue] == X;
    
}


- (void)lz_deviceJailBroken{
    if (check_jailbroken_sandbox() || check_jailbroken_tweak() || check_jailbroken_cydia() || check_jailbroken_dylib() || check_jailbroken_dyld()) {
        
        //1.退出app
//        found_jailbroken_device();
        
        //2.不可操作的提示
        if (lz_SYSTEM_VERSION_THAN(8.0, >=)) {
            dispatch_async(dispatch_get_main_queue(), ^{
                UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"错误" message:@"app不支持越狱或破解版的手机." preferredStyle:UIAlertControllerStyleAlert];
                [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:alert animated:YES completion:nil];
            });
            
            
        }else{
            [[[UIAlertView alloc] initWithTitle:@"错误" message:@"app不支持越狱或破解版的手机." delegate:nil cancelButtonTitle:nil otherButtonTitles:nil, nil] show];
        }
        //[NSThread sleepUntilDate:[NSDate date]];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
