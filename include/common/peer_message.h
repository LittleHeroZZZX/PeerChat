#pragma once

#include <ctime>
#include <string>
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/string.hpp"

struct SenderInfo {
    std::string name = "unknown";

    SenderInfo() = default;
    SenderInfo(SenderInfo&& other);
    SenderInfo& operator=(SenderInfo&& other);

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(name));
    }
};

enum MessageContentType { MSG_TEXT, MSG_IMAGE, MSG_FILE };

class Message {
  public:
    Message(Message&& other);
    Message& operator=(Message&& other);

    static Message make_text(SenderInfo sender, std::string content);
    static Message deserialize(const std::string& data);
    std::string serialize() const;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(sender),
           CEREAL_NVP(content),
           CEREAL_NVP(timestamp),
           CEREAL_NVP(content_type));
    }

  private:
    Message(SenderInfo sender,
            std::string content,
            std::time_t timestamp,
            MessageContentType type);
    Message() = default;

    SenderInfo sender;
    std::string content;
    std::time_t timestamp;
    MessageContentType content_type;
};
