#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#define int long long

vector<string> read_file(string filename){
    vector<string> lines;
    ifstream file(filename);
    string line;

    if(!file.is_open()){
        cout << "ERROR: Could not open " << filename << ". Check your directory path." << endl;
        return lines;
    }
    while(getline(file,line)){
        lines.push_back(line);
    }
    return lines;
}

int diff_algo(const vector<string>& A,const vector<string>& B,vector<vector<int>>& trace){
    int n=A.size();
    int m=B.size();
    vector<int> dp(n+m+1,0);
    int ans=-1;
    for (int d=0;d<=m+n;d++){
        for(int k=-d;k<=d;k+=2){
            if(k<-m||k>n) continue;
            int x;

            if(k==-m||k==-d){
                x=dp[m+k+1];
            }
            else if(k==n||k==d){
                x=dp[m+k-1]+1;
            }
            else{
                if(dp[m+k-1]>dp[m+k+1]){
                    x=dp[m+k-1]+1;
                }
                else{
                    x=dp[m+k+1];
                }
            }
            int y=x-k;

            while(x<n&&y<m&&A[x]==B[y]){
                x++;
                y++;
            }
            dp[m+k]=x;
            if(x>=n&&y>=m){
                ans=d;
                break;
            }
        }
        trace.push_back(dp);
        if(ans!=-1) break;
    }

    return ans;
}

void print_difference(const vector<string>& A, const vector<string>& B,const vector<vector<int>>& trace,int d){
    if(d==-1||trace.empty()) return ;
    int n=A.size();
    int m=B.size();
    int max=n+m+1;

    vector<string> result;

    int x=n,y=m;

    for (; d > 0; d--) {
        int k = x - y;
        const vector<int>& prev_dp = trace[d - 1];
        
        bool moveDown;
        if (k == -d || k == -m) {
            moveDown = true;
        } else if (k == d || k == n) {
            moveDown = false;
        } else {
            moveDown = (prev_dp[m + k - 1] < prev_dp[m + k + 1]);
        }
        
        int prev_k = moveDown ? k + 1 : k - 1;
        int prev_x = prev_dp[m + prev_k];
        int prev_y = prev_x - prev_k;
        
        while (x > prev_x && y > prev_y) {
            result.push_back("  " + A[x - 1]);
            x--;
            y--;
        }
        
        if (moveDown) {
            result.push_back("+ " + B[prev_y]);
        } else {
            result.push_back("- " + A[prev_x]);
        }
        
        x = prev_x;
        y = prev_y;
    }
    
    while (x > 0 && y > 0) {
        result.push_back("  " + A[x - 1]);
        x--;
        y--;
    }
    
    for (int i = result.size() - 1; i >= 0; i--) {
        if (result[i][0] == '+') {
            cout << "\033[32m" << result[i] << "\033[0m\n"; // Green
        } else if (result[i][0] == '-') {
            cout << "\033[31m" << result[i] << "\033[0m\n"; // Red
        } else {
            cout << result[i] << '\n'; // White
        }
    }
}


signed main(){
    vector<string> A=read_file("old.txt");
    vector<string> B=read_file("new.txt");

    vector<vector<int>> trace;
    int d=diff_algo(A,B,trace);
    cout<<d<<'\n';
    print_difference(A,B,trace,d);
    return 0;
}