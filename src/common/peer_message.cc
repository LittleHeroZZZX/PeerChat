#include "peer_message.h"
#include "cereal/archives/portable_binary.hpp"

Message::Message(Message&& other)
    : sender(std::move(other.sender)),
      content(std::move(other.content)),
      timestamp(other.timestamp),
      content_type(other.content_type) {}

Message& Message::operator=(Message&& other) {
    if (this != &other) {
        sender = std::move(other.sender);
        content = std::move(other.content);
        timestamp = other.timestamp;
        content_type = other.content_type;
    }
    return *this;
}

Message Message::make_text(SenderInfo sender, std::string content) {
    std::time_t timestamp = std::time(nullptr);
    return Message(std::move(sender),
                   std::move(content),
                   timestamp,
                   MessageContentType::MSG_TEXT);
}

Message Message::deserialize(const std::string& data) {
    std::stringstream ss(data);
    Message msg;
    {
        cereal::PortableBinaryInputArchive archive(ss);
        archive(msg);
    }
    return msg;
}

std::string Message::serialize() const {
    std::stringstream ss;
    {
        cereal::PortableBinaryOutputArchive archive(ss);
        archive(*this);
    }
    return ss.str();
}
Message::Message(SenderInfo sender,
                 std::string content,
                 std::time_t timestamp,
                 MessageContentType type)
    : sender(std::move(sender)),
      content(std::move(content)),
      timestamp(timestamp),
      content_type(type) {}
SenderInfo::SenderInfo(SenderInfo&& other) : name(std::move(other.name)) {}

SenderInfo& SenderInfo::operator=(SenderInfo&& other) {
    if (this != &other) {
        name = std::move(other.name);
    }
    return *this;
}
