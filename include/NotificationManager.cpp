#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "Notification.hpp"
#include "User.hpp"

class NotificationManager {
private:
    std::unordered_map<User*, std::vector<std::unique_ptr<Notification>>> userNotifications;
    mutable std::mutex mtx; // Para thread safety

    // Padrão Singleton
    static NotificationManager* instance;
    NotificationManager() = default;

public:
    // Singleton
    static NotificationManager& getInstance();
    NotificationManager(const NotificationManager&) = delete;
    void operator=(const NotificationManager&) = delete;

    // Gerenciamento de Notificações
    void addNotification(std::unique_ptr<Notification> notification);
    std::vector<Notification*> getUnreadNotifications(User* user) const;
    std::vector<Notification*> getAllNotifications(User* user) const;
    void markAsRead(User* user, int notificationId);
    void clearAllNotifications(User* user);
    int getUnreadCount(User* user) const;

    // Persistência
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
};
