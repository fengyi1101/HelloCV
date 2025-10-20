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
            cerr << "�����޷����ļ� " << filePath << endl;
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
            cerr << "�����޷�д���ļ� " << filePath << endl;
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
            cout << "1. ����" << endl;
            cout << "2. ����" << endl;
            cout << "3. �˳�" << endl;
            cout << "��ѡ�����: ";

            int mode;
            cin >> mode;
            if (cin.fail() || mode == 3) 
            {
                if (mode == 3) cout << "�����˳���" << endl;
                else cout << "��Ч���룬�����˳���" << endl;
                break;
            }
            if (mode != 1 && mode != 2) 
            {
                cout << "��Чѡ������ԡ�" << endl;
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
        const string opStr = isEncrypt ? "����" : "����";

        cout << "\n--- ѡ������Դ ---\n"
            << "1. �������������ı�\n"
            << "2. ���ļ���ȡ\n"
            << "��ѡ��: ";
        int source;
        cin >> source;
        clearInputBuffer();

        cout << "\n--- ѡ���㷨 ---\n"
            << "1. ��������\n"
            << "2. ���(XOR)����\n"
            << "��ѡ��: ";
        int algo;
        cin >> algo;
        clearInputBuffer();

        string text_in, text_out, key_str;
        int key_int = 0;

        if (source == 1) 
        {
            cout << "������Ҫ" << opStr << "���ı�: ";
            getline(cin, text_in);
        }
        else {
            string path;
            cout << "������Ҫ" << opStr << "���ļ�·��: ";
            getline(cin, path);
            text_in = FileHandler::readFile(path);
            if (text_in.empty()) return;
        }

        if (algo == 1) 
        {
            cout << "��������Կ (����): ";
            cin >> key_int;
            clearInputBuffer();
            int final_key = isEncrypt ? key_int : -key_int;
            text_out = Crypto::caesarCipher(text_in, final_key);
        }
        else 
        {
            cout << "��������Կ (�ַ���): ";
            getline(cin, key_str);
            text_out = Crypto::xorCipher(text_in, key_str);
        }

        if (source == 1) 
        {
            cout << opStr << "���: " << text_out << endl;
        }
        else 
        {
            const string out_path = "encrypted.txt";
            if (FileHandler::writeFile(out_path, text_out)) 
            {
                cout << opStr << "����ѱ��浽: " << out_path << endl;
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
