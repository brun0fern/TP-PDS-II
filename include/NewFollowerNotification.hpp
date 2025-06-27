#pragma once
#include "Notification.hpp"

class NewFollowerNotification : public Notification {
public:
    // Construtor
    NewFollowerNotification(User* recipient, User* newFollower);
    
    // Implementação dos métodos virtuais
    std::string getMessage() const override;
    NotificationType getType() const override;
    std::string serialize() const override;
    
    // Factory method
    static std::unique_ptr<NewFollowerNotification> deserialize(const std::string& data);
};
