#include "User.hpp"
#include "Tweet.hpp"
#include "Notification.hpp"
#include <algorithm>
#include <stdexcept>
#include <bcrypt/BCrypt.hpp>

// Construtor
User::User(const std::string& username, const std::string& password)
    : username(username) {
    if (!password.empty()) {
        this->password = encryptPassword(password);
    }
}

// Destrutor
User::~User() {
    for (auto* notification : notifications) {
        delete notification;
    }
}

// --- Gerenciamento de Tweets ---
Tweet* User::postTweet(const std::string& content) {
    Tweet* tweet = new Tweet(tweets.size() + 1, this, content);
    tweets.push_back(tweet);
    return tweet;
}

Tweet* User::postRetweet(Tweet* original, const std::string& comment) {
    if (!original) throw std::invalid_argument("Tweet original inválido");
    
    std::string content = "RT @" + original->getAuthor()->getUsername() + ": " + original->getContent();
    if (!comment.empty()) {
        content += " (" + comment + ")";
    }
    
    return postTweet(content);
}

// --- Rede Social ---
void User::follow(User* user) {
    if (!user || user == this) return;
    following.insert(user);
}

void User::unfollow(User* user) {
    following.erase(user);
}

bool User::isFollowing(const User* user) const {
    return following.find(const_cast<User*>(user)) != following.end();
}

// --- Notificações ---
void User::addNotification(Notification* notification) {
    if (notificationsEnabled && notification) {
        notifications.push_back(notification);
    }
}

void User::clearNotifications() {
    notifications.clear();
}

std::vector<Notification*> User::getUnreadNotifications() const {
    std::vector<Notification*> unread;
    for (auto* notif : notifications) {
        if (!notif->isRead()) {
            unread.push_back(notif);
        }
    }
    return unread;
}

int User::getUnreadNotificationCount() const {
    return getUnreadNotifications().size();
}

// --- Getters ---
std::string User::getUsername() const {
    return username;
}

std::vector<Tweet*> User::getTweets() const {
    return tweets;
}

std::vector<User*> User::getFollowing() const {
    return std::vector<User*>(following.begin(), following.end());
}

bool User::checkPassword(const std::string& pwd) const {
    return BCrypt::validatePassword(pwd, password);
}

// --- Configurações ---
void User::enableNotifications(bool enable) {
    notificationsEnabled = enable;
}

// --- Métodos Privados ---
std::string User::encryptPassword(const std::string& pwd) {
    return BCrypt::generateHash(pwd, 12);
}
