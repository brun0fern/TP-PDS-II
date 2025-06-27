#pragma once
#include "Tweet.hpp"
#include <string>

class Retweet : public Tweet {
private:
    Tweet* originalTweet;
    std::string comment;

public:
    // Construtor
    Retweet(int id, User* author, Tweet* original, const std::string& comment = "");
    
    // Sobrescrita de métodos
    void display() const override;
    
    // Getters
    Tweet* getOriginalTweet() const;
    std::string getComment() const;
    
    // Validação
    static bool isValidComment(const std::string& comment);
};
