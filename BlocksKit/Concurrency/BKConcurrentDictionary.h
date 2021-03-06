//
//  BKConcurrentDictionary.h
//  BlocksKit
//
//  Created by Andrew Romanov on 11/07/2019.
//  Copyright © 2019 Zachary Waldowski and Pandamonia LLC. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface BKConcurrentDictionary <KeyType, ObjectType> : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithDictionary:(NSDictionary<KeyType, ObjectType>*)dict;

/** Loops through the dictionary and executes the given block using each item.
 
 @param block A block that performs an action using a key/value pair.
 */
- (void)bk_each:(void (^)(KeyType key, ObjectType obj))block;

/** Enumerates through the dictionary concurrently and executes
 the given block once for each pair.
 
 Enumeration will occur on appropriate background queues;
 the system will spawn threads as need for execution. This
 will have a noticeable speed increase, especially on dual-core
 devices, but you *must* be aware of the thread safety of the
 objects you message from within the block.
 
 @param block A block that performs an action using a key/value pair.
 */
- (void)bk_apply:(void (^)(KeyType key, ObjectType obj))block __attribute((deprecated("Use bk_each")));

/** Loops through a dictionary to find the first key/value pair matching the block.
 
 bk_match: is functionally identical to bk_select:, but will stop and return
 the value on the first match.
 
 @param block A BOOL-returning code block for a key/value pair.
 @return The value of the first pair found;
 */
- (nullable ObjectType)bk_match:(BOOL (^)(KeyType key, ObjectType obj))block;

/** Loops through a dictionary to find the key/value pairs matching the block.
 
 @param block A BOOL-returning code block for a key/value pair.
 @return Returns a dictionary of the objects found.
 */
- (NSDictionary<KeyType, ObjectType> *)bk_select:(BOOL (^)(KeyType key, ObjectType obj))block;

/** Loops through a dictionary to find the key/value pairs not matching the block.
 
 This selector performs *literally* the exact same function as bk_select: but in reverse.
 
 This is useful, as one may expect, for filtering objects.
 NSDictionary *strings = [userData bk_reject:^BOOL(id key, id value) {
 return ([obj isKindOfClass:[NSString class]]);
 }];
 
 @param block A BOOL-returning code block for a key/value pair.
 @return Returns a dictionary of all objects not found.
 */
- (NSDictionary<KeyType, ObjectType> *)bk_reject:(BOOL (^)(KeyType key, ObjectType obj))block;

/** Call the block once for each object and create a dictionary with the same keys
 and a new set of values.
 
 @param block A block that returns a new value for a key/value pair.
 @return Returns a dictionary of the objects returned by the block.
 */
- (NSDictionary *)bk_map:(id (^)(KeyType key, ObjectType obj))block;

/** Loops through a dictionary to find whether any key/value pair matches the block.
 
 This method is similar to the Scala list `exists`. It is functionally
 identical to bk_match: but returns a `BOOL` instead. It is not recommended
 to use bk_any: as a check condition before executing bk_match:, since it would
 require two loops through the dictionary.
 
 @param block A two-argument, BOOL-returning code block.
 @return YES for the first time the block returns YES for a key/value pair, NO otherwise.
 */
- (BOOL)bk_any:(BOOL (^)(KeyType key, ObjectType obj))block;

/** Loops through a dictionary to find whether no key/value pairs match the block.
 
 This selector performs *literally* the exact same function as bk_all: but in reverse.
 
 @param block A two-argument, BOOL-returning code block.
 @return YES if the block returns NO for all key/value pairs in the dictionary, NO otherwise.
 */
- (BOOL)bk_none:(BOOL (^)(KeyType key, ObjectType obj))block;

/** Loops through a dictionary to find whether all key/value pairs match the block.
 
 @param block A two-argument, BOOL-returning code block.
 @return YES if the block returns YES for all key/value pairs in the dictionary, NO otherwise.
 */
- (BOOL)bk_all:(BOOL (^)(KeyType key, ObjectType obj))block;

@end


@interface NSDictionary <KeyType, ObjectType> (BlocksKit_Concurrent)

- (BKConcurrentDictionary<KeyType, ObjectType>*)bk_concurrent;

@end


NS_ASSUME_NONNULL_END
