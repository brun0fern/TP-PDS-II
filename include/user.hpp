#pragma once
#include <string>
#include <vector>
#include <set>
#include <memory>
#include "Tweet.hpp"
#include "Notification.hpp"

// Forward declarations
class Tweet;
class Notification;

class User {
private:
    std::string username;
    std::string password; // Hash da senha
    std::set<User*> following;
    std::vector<Tweet*> tweets;
    std::vector<Notification*> notifications;
    bool notificationsEnabled = true;

    // Métodos privados
    std::string encryptPassword(const std::string& pwd);

public:
    // Construtor/Destrutor
    User(const std::string& username, const std::string& password = "");
    ~User();

    // Gerenciamento de tweets
    Tweet* postTweet(const std::string& content);
    Tweet* postRetweet(Tweet* original, const std::string& comment = "");

    // Rede social
    void follow(User* user);
    void unfollow(User* user);
    bool isFollowing(const User* user) const;

    // Notificações
    void addNotification(Notification* notification);
    void clearNotifications();
    std::vector<Notification*> getUnreadNotifications() const;
    int getUnreadNotificationCount() const;

    // Getters
    std::string getUsername() const;
    std::vector<Tweet*> getTweets() const;
    std::vector<User*> getFollowing() const;
    bool checkPassword(const std::string& pwd) const;

    // Configurações
    void enableNotifications(bool enable);
};
