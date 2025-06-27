#pragma once
#include "Notification.hpp"
#include "Tweet.hpp"

class MentionNotification : public Notification {
private:
    Tweet* mentionedTweet; // Tweet que contém a menção

public:
    // Construtor
    MentionNotification(User* recipient, User* sender, Tweet* tweet);
    
    // Getters
    Tweet* getMentionedTweet() const;
    
    // Implementação dos métodos virtuais
    std::string getMessage() const override;
    NotificationType getType() const override;
    std::string serialize() const override;
    
    // Factory method para desserialização
    static std::unique_ptr<MentionNotification> deserialize(const std::string& data);
};
