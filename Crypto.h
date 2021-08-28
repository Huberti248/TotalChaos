#pragma once
#include <iostream>
#include <string>
#include <vector>

class Crypto {
public:
	Crypto() {}
	~Crypto() {}

	static std::string Encrypt(const std::string& msg, std::string& key);

	static std::string Decrypt(std::string& encryptedMsg, std::string& key);

private:
	inline static std::string const AVAILABLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
	
	static int Index(char c);

	static std::string Extend_key(std::string& msg, std::string& key);

	static std::string Encrypt_vigenere(std::string& msg, std::string& key);

	static std::string Decrypt_vigenere(std::string& encryptedMsg, std::string& key);

	static std::string base64_encode(const std::string& in);

	static std::string base64_decode(const std::string& in);
};