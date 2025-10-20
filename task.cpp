#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

class Crypto 
{
public:
    static string caesarCipher(const string& text, int key) 
    {
        string result = "";
        for (char c : text) 
        {
            if (isalpha(c)) 
            {
                char base = isupper(c) ? 'A' : 'a';
                result += static_cast<char>((c - base + key % 26 + 26) % 26 + base);
            }
            else 
            {
                result += c;
            }
        }
        return result;
    }

    static string xorCipher(const string& text, const string& key) 
    {
        if (key.empty()) return text;
        string result = text;
        for (size_t i = 0; i < text.length(); ++i) 
        {
            result[i] = text[i] ^ key[i % key.length()];
        }
        return result;
    }
};

class FileHandler 
{
public:
    static string readFile(const string& filePath) 
    {
        ifstream file(filePath, ios::binary);
        if (!file)
        {
            cerr << "错误：无法打开文件 " << filePath << endl;
            return "";
        }
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static bool writeFile(const string& filePath, const string& content) 
    {
        ofstream file(filePath, ios::binary);
        if (!file) 
        {
            cerr << "错误：无法写入文件 " << filePath << endl;
            return false;
        }
        file << content;
        return true;
    }
};

class Menu 
{
public:
    void run() 
    {
        while (true) 
        {
            cout << "\n===== CryptoTool =====" << endl;
            cout << "1. 加密" << endl;
            cout << "2. 解密" << endl;
            cout << "3. 退出" << endl;
            cout << "请选择操作: ";

            int mode;
            cin >> mode;
            if (cin.fail() || mode == 3) 
            {
                if (mode == 3) cout << "程序退出。" << endl;
                else cout << "无效输入，程序退出。" << endl;
                break;
            }
            if (mode != 1 && mode != 2) 
            {
                cout << "无效选项，请重试。" << endl;
                clearInputBuffer();
                continue;
            }

            process(mode == 1);
        }
    }

private:
    void clearInputBuffer() 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void process(bool isEncrypt) 
    {
        const string opStr = isEncrypt ? "加密" : "解密";

        cout << "\n--- 选择数据源 ---\n"
            << "1. 从命令行输入文本\n"
            << "2. 从文件读取\n"
            << "请选择: ";
        int source;
        cin >> source;
        clearInputBuffer();

        cout << "\n--- 选择算法 ---\n"
            << "1. 凯撒密码\n"
            << "2. 异或(XOR)密码\n"
            << "请选择: ";
        int algo;
        cin >> algo;
        clearInputBuffer();

        string text_in, text_out, key_str;
        int key_int = 0;

        if (source == 1) 
        {
            cout << "请输入要" << opStr << "的文本: ";
            getline(cin, text_in);
        }
        else {
            string path;
            cout << "请输入要" << opStr << "的文件路径: ";
            getline(cin, path);
            text_in = FileHandler::readFile(path);
            if (text_in.empty()) return;
        }

        if (algo == 1) 
        {
            cout << "请输入密钥 (整数): ";
            cin >> key_int;
            clearInputBuffer();
            int final_key = isEncrypt ? key_int : -key_int;
            text_out = Crypto::caesarCipher(text_in, final_key);
        }
        else 
        {
            cout << "请输入密钥 (字符串): ";
            getline(cin, key_str);
            text_out = Crypto::xorCipher(text_in, key_str);
        }

        if (source == 1) 
        {
            cout << opStr << "结果: " << text_out << endl;
        }
        else 
        {
            const string out_path = "encrypted.txt";
            if (FileHandler::writeFile(out_path, text_out)) 
            {
                cout << opStr << "结果已保存到: " << out_path << endl;
            }
        }
    }
};

int main() 
{
    Menu menu;
    menu.run();
    return 0;
}
