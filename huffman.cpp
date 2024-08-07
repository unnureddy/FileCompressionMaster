#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int freq = 0;
    char c = '\0';
    Node *left = NULL, *right = NULL;
    Node(int freq, char c): freq(freq), c(c) {}
    Node(int freq, char c, Node *left, Node *right): freq(freq), c(c), left(left), right(right) {}
    bool is_leaf()
    {
        return left == NULL && right == NULL;
    }
};

Node* _build_repr_tree(vector<string> &repr);
int compress(string input_file, string output_file), decompress(string input_file, string output_file), _compress_file(ifstream &in, ofstream &out), decompress_file(ifstream &in, ofstream &out), _decompress_file(ifstream &in, ofstream &out);
void help_banner(), _get_huffman_representation(Node* root, vector<string>& repr, string s);

int main(int argc, char *argv[])
{
    if (argc>1)
    {
        string command = argv[1];
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        if (command == "compress" || command == "c")
        {
            string input_file = argv[2];
            string output_file = argv[3];
            cout << "Compressing " << input_file << " to " << output_file << endl;
            compress(input_file, output_file);
        }
        else if (command == "decompress" || command == "d")
        {
            string input_file = argv[2];
            string output_file = argv[3];
            cout << "Decompressing " << input_file << " to " << output_file << endl;
            decompress(input_file, output_file);
        }
        else if (command == "help" || command == "h" || command == "--h")
            help_banner();
        else
            cout << "Invalid command" << endl;
    }
    else
    {
        cout << "Welcome to Huffman Compression Program\nPress enter help(h) for instructions." << endl;
        string command, input_file, output_file;
        while (true)
        {
            cout << "\nEnter command: ";
            cin >> command;
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "compress" || command == "c")
            {
                cout << "Enter input file: ";
                cin >> input_file;
                cout << "Enter output file (without file extension): ";
                cin >> output_file;
                cout << "Compressing " << input_file << " to " << output_file + ".hufbin" << endl;
                compress(input_file, output_file);
            }
            else if (command == "decompress" || command == "d")
            {
                cout << "Enter input file (with .hufbin extension): ";
                cin >> input_file;
                cout << "Enter output file (with original file extension): ";
                cin >> output_file;
                cout << "Decompressing " << input_file << " to " << output_file << endl;
                decompress(input_file, output_file);
            }
            else if (command == "help"|| command == "h" || command == "--h")
                help_banner();
            else if (command == "exit")
            {
                cout << "\nExiting...\nThank you for using the program!" << endl;
                break;
            }
            else
                cout << "Invalid command" << endl;
        }
    }
    return 0;
}

void help_banner()
{
    cout<<"\nThis program implements huffman coding algorithm to compress and decompress any type of files."<<endl;
    cout<<"Usage: ./huffman [command] [input_file] [output_file]"<<endl;
    cout<<"Alternatively, you can run the program without any arguments and follow the instructions."<<endl;
    cout<<"Commands:"<<endl;
    cout<<"compress (c) - Compresses the input file and saves it as output file"<<endl;
    cout<<"decompress (d) - Decompresses the input file and saves it as output file"<<endl;
    cout<<"help (h) - Displays this help message"<<endl;
    cout<<"exit - Exits the program\n"<<endl;
}

int compress(string input_file, string output_file)
{
    ifstream in(input_file, ios::binary);
    if (!in)
    {
        cout << "Error opening file " << input_file << endl;
        return 1;
    }
    ofstream out(output_file + ".hufbin", ios::binary);
    if (!out)
    {
        cout << "Error opening file " << output_file << endl;
        return 1;
    }
    cout << "Compressing..." << endl;
    _compress_file(in, out);
    in.close();
    out.close();
    cout << "Compression complete!" << endl;
    return 0;
}

void _get_huffman_representation(Node* root, vector<string>& repr, string s = "")
{
    if (!root) return;
    if (root->is_leaf())
    {
        if (s=="") repr[(root->c+256)%256] = "0";
        else repr[(root->c+256)%256] = s;
        return;
    }
    _get_huffman_representation(root->left, repr, s + "0");
    _get_huffman_representation(root->right, repr, s + "1");
}

void _write_string(ofstream &out, string &s, bool pad = false)
{
    // cout<<s<<endl;
    static unsigned char temp = '\0';
    static int counter = 0;
    long long int size = s.size();
    for (long long int i = 0; i < size; i++)
    {
        temp = temp << 1 | (s[i]-'0');
        counter++;
        if (counter == 8)
            out.put(temp), counter = 0, temp = '\0';
    }
    if (counter && pad)
    {
        // cout<<"yes\n";
        while(counter!=8) counter++, temp <<= 1;
        counter = 0, out.put(temp);
    }
}

int _compress_file(ifstream &in, ofstream &out)
{
    vector<int> freq(256, 0);
    char c;
    long long int total_bytes = 0;
    while (in.get(c)) freq[(c+256)%256]++, total_bytes++;
    in.clear();
    in.seekg(0, ios::beg);
    // cout<<total_bytes<<endl;
    priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> huffman_heap([](Node *a, Node *b) { return a->freq > b->freq; });
    for (int i = 0; i < 256; i++)
        if (freq[i] > 0)
           huffman_heap.push(new Node(freq[i], i));
    int16_t num_freq_table_rows = huffman_heap.size();
    if (!num_freq_table_rows) return 0;
    while (huffman_heap.size() > 1)
    {
        Node *left = huffman_heap.top();
        huffman_heap.pop();
        Node *right = huffman_heap.top();
        huffman_heap.pop();
        huffman_heap.push(new Node(left->freq + right->freq, '\0', left, right));
    }
    // For storing the representations of each byte. String because some character might be very rare and it's representation can exceed 64 bit allowed by integers.
    vector<string> repr(256);
    _get_huffman_representation(huffman_heap.top(), repr);
    // 2 byte number for how many entries in frequency table
    out.put(num_freq_table_rows>>8);
    out.put(num_freq_table_rows%256);
    // Byte value, 16 bits for length of compressed representation bits, New representation with padded 0s to right
    for (int i = 0; i < 256; i++)
        if (repr[(i+256)%256]!="")
        {
            out.put(i);
            // cout<<i<<" "<<repr[i]<<endl;
            int16_t num_of_bits = repr[i].size();
            out.put((num_of_bits>>8));
            out.put((num_of_bits%256));
            _write_string(out, repr[i], true);
        }
    // Now 64-bits telling total number of bytes of the input file
    for (int i = 56 ; i >=0 ; i -= 8) out.put((total_bytes>>i) % 256);
    // cout<<"Done\n";
    int count = 0;
    // Then the compressed data till end. 
    while (in.get(c))
        _write_string(out, repr[(c+256)%256]),count++;
    // cout<<count<<endl;
    // Then the last byte which might not be full
    string temp;
    _write_string(out, temp, true);
    return 0;
}

int decompress(string input_file, string output_file)
{
    ifstream in(input_file, ios::binary);
    if (!in)
    {
        cout << "Error opening file " << input_file << endl;
        return 1;
    }
    ofstream out(output_file, ios::binary);
    if (!out)
    {
        cout << "Error opening file " << output_file << endl;
        return 1;
    }
    cout << "Decompressing..." << endl;
    _decompress_file(in, out);
    in.close();
    out.close();
    cout << "Decompression complete!" << endl;
    return 0;
    return 0;
}

int _decompress_file(ifstream &in, ofstream&out)
{
    // First 2 bytes for number of entries in frequency table
    int16_t num_freq_table_rows = 0;
    if (!in.get((char&)num_freq_table_rows)) return 0;
    num_freq_table_rows <<= 8;
    in.get((char&)num_freq_table_rows);
    // cout<<num_freq_table_rows<<endl;
    if (!num_freq_table_rows) return 0;
    // Then the representation of each byte
    vector<string> repr(256);
    char c, f;
    for (int i = 0; i < num_freq_table_rows; i++)
    {
        int16_t num_of_bits = 0;
        in.get(c);
        // cout<<c<<" "<<(int)c<<endl;
        in.get((char&)num_of_bits);
        // cout<<num_of_bits<<endl;
        num_of_bits <<= 8;
        in.get((char&)num_of_bits);
        // cout<<num_of_bits<<endl;
        for (int j = 0; j < num_of_bits;)
        {
            in.get(f);
            // cout<<"kk  "<<f<<endl;
            for (int k = 0; k < 8 && j < num_of_bits; k++, j++)
                repr[(c+256)%256].push_back(((f >> (7-k)) & 1) + '0');
        }
    }
    // cout<<"Done reading"<<endl;
    Node* root = _build_repr_tree(repr);
    // Then the total number of bytes of the input file
    long long int total_bytes = 0;
    for (int i = 0; i < 8; i++)
    {
        in.get(c);
        // cout<<"bytes "<<c<<" "<<(int)c<<endl;
        total_bytes <<= 8;
        total_bytes |= (long long int)(c+256)%256;
    }
    int bytes_read = 0;
    Node* temp = root;
    // cout<<total_bytes<<endl;
    // Then the compressed data
    while (bytes_read < total_bytes)
    {
        if (!in.get(c)) break;
        for (int i = 0; i < 8 && bytes_read < total_bytes; i++)
        {
            if (c & (1 << (7-i))) temp = temp->right;
            else temp = temp->left;
            if (temp->is_leaf())
            {
                out.put(temp->c);
                temp = root;
                bytes_read++;
            }
        }
    }
    return 0;
}

Node* _build_repr_tree(vector<string> &repr)
{
    Node *root = new Node(0, '\0');
    for (int i = 0; i < 256; i++)
    {
        if (repr[i] == "") continue;
        Node *temp = root;
        for (int j = 0; j < repr[i].size(); j++)
        {
            if (repr[i][j] == '0')
            {
                if (!temp->left) temp->left = new Node(0, '\0');
                temp = temp->left;
            }
            else
            {
                if (!temp->right) temp->right = new Node(0, '\0');
                temp = temp->right;
            }
        }
        if (!temp->is_leaf())
        {
            cout<<"File corrupted!"<<endl;
            exit(1);
        }
        temp->c = (char)i;
    }
    return root;
}


