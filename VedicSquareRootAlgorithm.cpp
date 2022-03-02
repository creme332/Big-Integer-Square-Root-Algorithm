#include <iostream>
#include <vector>
#include <cmath>
#include <regex>
#include <string>

using namespace std;

string add(string a, string b)
{
    a = string(max(a.size(), b.size()) + 1 - a.size(), '0') + a;
    b = string(a.size() - b.size(), '0') + b;
    for (int i = a.size() - 1, carry = 0; i >= 0; i--)
    {
        int sum = a[i] + b[i] - 96 + carry;
        carry = sum / 10;
        a[i] = sum % 10 + '0';
    }
    int i = a.find_first_not_of('0');
    return 0 <= i ? a.substr(i) : a.substr(0, 1);
}
string multiply(const string& t, const string& b)
{ 
    //t and b can be very large
    int tsize = static_cast<int>(t.size());
    int bsize = static_cast<int>(b.size());

    vector<size_t> pos(static_cast<size_t>(tsize + bsize), 0);
    for (int i = tsize - 1; i >= 0; i--) {
        for (int j = bsize - 1; j >= 0; j--) {
            int mul = (t[i] - '0') * (b[j] - '0');
            int p1 = i + j, p2 = i + j + 1;
            int sum = mul + pos[p2];
            pos[p1] += sum / 10;
            pos[p2] = sum % 10;
        }
    }
    string res;
    for (const auto& p : pos) if (!(res.size() == 0 && p == 0)) res.push_back('0' + p);

    return (res.size() == 0) ? "0" : res;
}
string subtract(string a, string b) {
    string str = "";

    int n1 = a.length(), n2 = b.length();
    int diff = n1 - n2;
    int carry = 0;

    for (int i = n2 - 1; i >= 0; i--) {
        int sub = ((a[i + diff] - '0') - (b[i] - '0')
            - carry);
        if (sub < 0) {
            sub = sub + 10;
            carry = 1;
        }
        else
            carry = 0;

        str.push_back(sub + '0');
    }

    // subtract remaining digits of str1[]
    for (int i = n1 - n2 - 1; i >= 0; i--) {
        if (a[i] == '0' && carry) {
            str.push_back('9');
            continue;
        }
        int sub = ((a[i] - '0') - carry);
        if (i > 0 || sub > 0) // remove preceding 0's
            str.push_back(sub + '0');
        carry = 0;
    }

    reverse(str.begin(), str.end());

    //remove leading zeroes
    int i = str.find_first_not_of('0');
    return 0 <= i ? str.substr(i) : str.substr(0, 1);
}

bool compare(string a, string b) {
    //returns 1 if a > b else return 0;
    // a and b are very large numbers
    if (a.length() > b.length()) { return 1;}
    if (a.length() < b.length()) { return 0;}

    a = string(max(a.size(), b.size()) + 1 - a.size(), '0') + a;
    b = string(a.size() - b.size(), '0') + b;
    for (int i = 0;i < a.length();i++) {
        if (int(a[i]-'0') > int(b[i]-'0')) { return 1; }
        if (int(a[i] - '0') < int(b[i] - '0')) { return 0; }
    }
    return 0;

}

string removeleadingzeroes(string str) {
    //remove leading zeroes
    int i = str.find_first_not_of('0');
    return 0 <= i ? str.substr(i) : str.substr(0, 1);
}

string vedic_square_root(const string s, long long precision) {
    const long long n = s.length(); 
    if(s.length()<2){ // USE 19. s is still small so use in-built square root function instead
        long long ans = sqrt(stoll(s));
        return to_string(ans);
    }
    string ans="", group="", col="";
    long long i; //index

    if (n % 2 == 0) { //leftmost group contains 2 digits and all other groups contain 2 digits
        group += to_string(s[0]-'0') + to_string(s[1]-'0');
        i = 2;
    }
    else { //leftmost gtroup contains 1 digit and other groups contain 2 digits
        group += to_string(s[0] - '0');
        i = 1;
    }

    //initialise ans, col, group
    ans = to_string(int(sqrt(stoi(group))));
    col = to_string(2 * stoi(ans));
    group = to_string(stoi(group) - stoi(ans) * stoi(ans)); //group = group - ans^2

    bool AddDecimalPoint=0;

    while (i < s.length() - 1 || precision > 0) {
        if(i < s.length() - 1 ){
            //drop 2 digits from s
            group += to_string(s[i] - '0') + to_string(s[i + 1] - '0');
            group = removeleadingzeroes(group);
            i+=2;
            if(i>=s.length() - 1){AddDecimalPoint=1;}
        }else{
            group+="00";
            precision--;
        }


        //We must now find the largest value of k such that (col(k)) * k <= group 
        //can be optimised using binary search algorithm
        int k = 9;
        string newstr = col + to_string(k); // newstr = (col(k))
        string prod = multiply(newstr, to_string(k));
        while (compare(prod, group) == 1) {
            k--;
            newstr = col + to_string(k);
            prod = multiply(newstr, to_string(k));
        }

        ans += to_string(k);
        col += to_string(k);

        group = subtract(group, prod); //group =  group - prod
        //new col = last digit of col + col
        col = add(col, to_string(int(col[col.length() - 1] - '0')));
        if(AddDecimalPoint){ans+=".";AddDecimalPoint=0;}
    }

    return ans;
}

int main(){
     //cout<<vedic_square_root("2930455",100); //Does not round off 
    string col="3", group="54";
 
    //find the largest integer value of k such that (col(k)) * k <= group 
    // largest(3,54) = 1
    int k = 0;
    string colk, prod;

    for(int jump = 5; jump>=1; jump/=2){
        colk=col+to_string(k);
        prod = multiply(colk, to_string(k));
        while(k+jump<10 && compare(group, prod)){k+=jump;}
    }
    cout<<k<<"\n";
}