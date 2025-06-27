#pragma once
#include <string>
#include <vector>
#include <set>
#include <ctime>
#include <memory>
#include "User.hpp"

// Forward declaration
class User;

class Tweet {
protected:
    int id;
    User* author;
    std::string content;
    std::time_t timestamp;
    std::set<std::string> hashtags;
    std::set<std::string> mentions;
    std::vector<std::string> comments;
    int likes = 0;
    bool isRetweet = false;

    void extractMetadata();

public:
    // Construtores
    Tweet(int id, User* author, const std::string& content);
    virtual ~Tweet() = default;

    // Métodos principais
    virtual void display() const;
    void like();
    void addComment(const std::string& comment);
    
    // Getters
    int getId() const;
    User* getAuthor() const;
    std::string getContent() const;
    std::string getFormattedTime() const;
    int getLikeCount() const;
    std::vector<std::string> getComments() const;
    std::set<std::string> getHashtags() const;
    std::set<std::string> getMentions() const;
    bool getIsRetweet() const;

    // Métodos estáticos
    static bool isValidContent(const std::string& content);
};

class Retweet : public Tweet {
private:
    Tweet* originalTweet;
    std::string comment;

public:
    Retweet(int id, User* author, Tweet* original, const std::string& comment = "");
    void display() const override;
    Tweet* getOriginalTweet() const;
};
