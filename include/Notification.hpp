#pragma once
#include <string>
#include <vector>
#include <chrono>
#include "User.hpp"

// Tipos de notificação suportados
enum class NotificationType {
    NEW_FOLLOWER,
    MENTION,
    RETWEET,
    LIKE
};

// Classe base abstrata
class Notification {
protected:
    User* recipient;
    User* sender;
    std::chrono::system_clock::time_point timestamp;
    bool read = false;

public:
    Notification(User* recipient, User* sender);
    virtual ~Notification() = default;
    
    virtual std::string getMessage() const = 0;
    virtual NotificationType getType() const = 0;
    
    void markAsRead();
    bool isRead() const;
    std::string getTimestamp() const;
};

// Notificação específica: Menção
class MentionNotification : public Notification {
    Tweet* mentionedTweet;

public:
    MentionNotification(User* recipient, User* sender, Tweet* tweet);
    std::string getMessage() const override;
    NotificationType getType() const override;
};

// Outras notificações (LikeNotification, RetweetNotification...) 
// [Implementações similares]
