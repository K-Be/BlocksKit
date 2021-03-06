//
//  BKLockTest.m
//  BlocksKit
//
//  Created by Andrew Romanov on 11/07/2019.
//  Copyright © 2019 Zachary Waldowski and Pandamonia LLC. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "BKLock.h"


@interface BKLockTest : XCTestCase

@end

@implementation BKLockTest

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testLock {
	NSMutableArray<NSNumber*>* objects = [NSMutableArray arrayWithCapacity:2];
	
	BKLock* sync = [[BKLock alloc] init];
	
	NSCondition* condition = [[NSCondition alloc] init];
	[condition lock];
	__block BOOL started = NO;
	dispatch_async(dispatch_get_global_queue(QOS_CLASS_DEFAULT, 0), ^{
		[sync exec:^{
			[condition lock];
			started = YES;
			[condition signal];
			[condition unlock];
			[NSThread sleepForTimeInterval:0.1];
			[objects addObject:@(1)];
		}];
	});
	if (!started)
	{
		[condition waitUntilDate:[NSDate dateWithTimeIntervalSinceNow:1000.0]];
	}
	else
	{
		[condition unlock];
	}
	
	[sync exec:^{
		[objects addObject:@(2)];
	}];
	
	BOOL equal = [objects isEqual:@[@(1), @(2)]];
	XCTAssertTrue(equal, @"must be equal arrays");
}

- (void)testPerformanceArray {
	BKLock* exec = [[BKLock alloc] init];
	NSInteger countObjects = 100000;
	NSMutableArray* objects = [[NSMutableArray alloc] initWithCapacity:countObjects];
	NSMutableArray<NSNumber*>* objectsToSet = [[NSMutableArray alloc] initWithCapacity:countObjects];
	for (NSInteger index = 0; index < countObjects; index++){
		[objectsToSet addObject:@(index)];
	}

	[self measureMetrics:self.class.defaultPerformanceMetrics
automaticallyStartMeasuring:NO
							forBlock:^{
								[self startMeasuring];
								[objectsToSet enumerateObjectsWithOptions:NSEnumerationConcurrent
																							 usingBlock:^(NSNumber* _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
																								 [exec exec:^{
																									 [objects addObject:obj];
																								 }];
																							 }];
								[self stopMeasuring];
								[objects removeAllObjects];
							}];
}

@end
