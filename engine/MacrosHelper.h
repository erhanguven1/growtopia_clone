//
// Created by erhan güven on 2.09.2023.
//

#ifndef GROWTOPIA_CLONE_MACROSHELPER_H
#define GROWTOPIA_CLONE_MACROSHELPER_H

#define OBFUSCATE_INT(value) ((value) * (-3) + 8771)
#define CLARIFY_INT(value) ((value) - 8771) / ((-3))

#define OBFUSCATED_CLASS_INT_W_DEFAULT(varName, funcName, defaultValue) \
private: int varName = OBFUSCATE_INT(defaultValue); \
public: int get##funcName() const { return CLARIFY_INT(varName); }; \
void set##funcName(int value) { varName = OBFUSCATE_INT(value); };

#define OBFUSCATED_CLASS_INT64_W_DEFAULT(varName, funcName, defaultValue) \
private: int64_t varName = OBFUSCATE_INT(defaultValue); \
public: int64_t get##funcName() const { return CLARIFY_INT(varName); }; \
void set##funcName(int64_t value) { varName = OBFUSCATE_INT(value); };

#define OBFUSCATED_CLASS_INT(varName, funcName) OBFUSCATED_CLASS_INT_W_DEFAULT(varName, funcName, 0)

#define OBFUSCATED_CLASS_INT64(varName, funcName) OBFUSCATED_CLASS_INT64

#endif //GROWTOPIA_CLONE_MACROSHELPER_H
