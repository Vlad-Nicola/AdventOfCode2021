#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

constexpr std::string hex2str(const unsigned char c) 
{
    switch (c) {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    default: return "1111";
    }
}

uint64_t str2dec(const char* str, const size_t len) 
{
    uint64_t result = 0;
    for (unsigned int i = 0; i < len; ++i) {
        result += str[i] - '0';
        result <<= 1;
    }
    result >>= 1;
    return result;
}

struct Packet
{
    Packet(const uint64_t version, const uint64_t typeID) : version(version), typeID(typeID) {}
    virtual uint64_t getVersion() const = 0;
    virtual uint64_t getResult() const = 0;

    uint64_t version = 0;
    uint64_t typeID = 0;
};

struct LVPacket : public Packet
{
    LVPacket(const uint64_t version, 
        const uint64_t typeID, 
        const uint64_t value) : Packet(version, typeID), value(value) {}
    uint64_t getVersion() const { return version; }
    uint64_t getResult() const
    {
        return value;
    }

    uint64_t value = 0;
};

struct OpPacket : public Packet
{
    OpPacket(const uint64_t version, const uint64_t typeID) : Packet(version, typeID) {}
    uint64_t getVersion() const {
        uint64_t totalVersionCount = version;
        for (const auto& p : subPackets) {
            totalVersionCount += p->getVersion();
        }
        return totalVersionCount;
    }
    uint64_t getResult() const
    {
        uint64_t res = 0;
        switch (typeID) {
        case 0:
            for (const auto p : subPackets) {
                res += p->getResult();
            }
            return res;
        case 1:
            res = 1;
            for (const auto p : subPackets) {
                res *= p->getResult();
            }
            return res;
        case 2:
            res = -1;
            for (const auto p : subPackets) {
                const auto pval = p->getResult();
                if (pval < res) {
                    res = pval;
                }
            }
            return res;
        case 3:
            for (const auto p : subPackets) {
                const auto pval = p->getResult();
                if (pval > res) {
                    res = pval;
                }
            }
            return res;
        case 5:
            return subPackets.front()->getResult() > subPackets.back()->getResult() ? 1 : 0;
        case 6:
            return subPackets.front()->getResult() < subPackets.back()->getResult() ? 1 : 0;
        default:
            return subPackets.front()->getResult() == subPackets.back()->getResult() ? 1 : 0;
        }
    }

    std::vector<Packet*> subPackets;
};

Packet* readPacket(std::stringstream& str)
{
    char versionStr[4], typeIDStr[4];
    str.get(versionStr, 4);
    str.get(typeIDStr, 4);

    if (strlen(typeIDStr) == 0) {
        return nullptr;
    }

    const uint64_t version = str2dec(versionStr, 3), typeID = str2dec(typeIDStr, 3);
    if (typeID == 4) {
        std::string valueStr;
        while (true) {
            char partStr[6];
            str.get(partStr, 6);
            valueStr += partStr + 1;
            if (partStr[0] == '0') {
                break;
            }
        }
        const uint64_t val = str2dec(valueStr.c_str(), valueStr.size());
        return new LVPacket(version, typeID, val);
    }
    else {
        OpPacket* packet = new OpPacket(version, typeID);
        char lengthTypeId;
        str.get(lengthTypeId);
        if (lengthTypeId == '0') {
            char bitCountStr[16];
            str.get(bitCountStr, 16);
            const uint64_t bitCount = str2dec(bitCountStr, 15);
            std::unique_ptr<char[]> s(new char[bitCount + 1]);
            str.get(s.get(), bitCount + 1);
            std::stringstream newStreamStr(s.get());
            while (!newStreamStr.eof()) {
                if (const auto val = readPacket(newStreamStr); val != nullptr) {
                    packet->subPackets.emplace_back(val);
                }
            }
        } else {
            char packetCountStr[12];
            str.get(packetCountStr, 12);
            uint64_t packetCount = str2dec(packetCountStr, 11);
            for (unsigned int i = 0; i < packetCount; ++i) {
                if (const auto val = readPacket(str); val != nullptr) {
                    packet->subPackets.emplace_back(val);
                }
            }
        }
        return packet;
    }
}

int main()
{
    std::ifstream file("input.txt");
    std::stringstream str;
    for (char c; file >> c; ) {
        str << hex2str(c);
    }

    Packet* packet = readPacket(str);
    uint64_t vers = packet->getVersion(), res = packet->getResult();

    std::cout << "(Part 1) Version sum: " << vers << std::endl;
    std::cout << "(Part 2) Result: " << res << std::endl;

    return vers == 843 && res == 5390807940351 ? 0 : -1;
}
