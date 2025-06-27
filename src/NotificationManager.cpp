#include "NotificationManager.hpp"
#include "MentionNotification.hpp"
#include "NewFollowerNotification.hpp"
#include "RetweetNotification.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

// Inicialização do Singleton
NotificationManager* NotificationManager::instance = nullptr;

NotificationManager& NotificationManager::getInstance() {
    static NotificationManager instance;
    return instance;
}

void NotificationManager::addNotification(std::unique_ptr<Notification> notification) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!notification) return;
    
    User* recipient = notification->getRecipient();
    userNotifications[recipient].push_back(std::move(notification));
}

std::vector<Notification*> NotificationManager::getUnreadNotifications(User* user) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<Notification*> result;
    
    if (userNotifications.find(user) != userNotifications.end()) {
        for (const auto& notif : userNotifications.at(user)) {
            if (!notif->isRead()) {
                result.push_back(notif.get());
            }
        }
    }
    return result;
}

std::vector<Notification*> NotificationManager::getAllNotifications(User* user) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<Notification*> result;
    
    if (userNotifications.find(user) != userNotifications.end()) {
        for (const auto& notif : userNotifications.at(user)) {
            result.push_back(notif.get());
        }
    }
    return result;
}

void NotificationManager::markAsRead(User* user, int notificationId) {
    std::lock_guard<std::mutex> lock(mtx);
    if (userNotifications.find(user) != userNotifications.end()) {
        for (auto& notif : userNotifications.at(user)) {
            if (notif->getId() == notificationId) {
                notif->markAsRead();
                break;
            }
        }
    }
}

void NotificationManager::clearAllNotifications(User* user) {
    std::lock_guard<std::mutex> lock(mtx);
    userNotifications.erase(user);
}

int NotificationManager::getUnreadCount(User* user) const {
    return getUnreadNotifications(user).size();
}

void NotificationManager::saveToFile(const std::string& filename) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo para salvar notificações");
    }
    
    for (const auto& pair : userNotifications) {
        for (const auto& notif : pair.second) {
            file << notif->serialize() << "\n";
        }
    }
}

void NotificationManager::loadFromFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream file(filename);
    
    if (!file.is_open()) return;
    
    std::string line;
    while (std::getline(file, line)) {
        auto notification = NotificationFactory::deserialize(line);
        if (notification) {
            User* recipient = notification->getRecipient();
            userNotifications[recipient].push_back(std::move(notification));
        }
    }
}
