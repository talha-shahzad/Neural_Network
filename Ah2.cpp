#include <iostream>
#include <pthread.h>
#include <future>
#include <string.h>
#include <cstring>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
struct input
{
    double *inp;
    int total = 0;
    input()
    {
        inp = NULL;
        total = 0;
    }
    input(int size)
    {
        inp = new double[size];
        total = 0;
    }
    void setsize(int s)
    {
        inp = new double[s];
    }
    void setval(double val)
    {
        inp[total] = val;
        total++;
    }
    double getval(int index)
    {
        if (index < total)
        {
            return this->inp[index];
        }
        return -999.9999;
    }
    void display()
    {
        for (int i = 0; i < total; i++)
            cout << inp[i] << " ";
        cout << endl;
    }
};
// hidden neuron=hd
struct hn
{
    double *input;
    double *weight;
    int size;
    int fin;
    int file_descriptor;
};
int read(string s, input &in, int neurons)
{
    ifstream file;
    file.open(s);
    string st;
    int count = 1;
    int a = 1, i = 0, move = 0;
    getline(file, st);
    cout << st << '\n';

    while (st[i] != '\0')
    {
        if (st[i] == ',')
            count++;
        i++;
    }
    in.setsize(count);
    // getline(file,st);
    file.close();
    cout << "total input " << count << endl;
    i = 0;
    char *d = new char[10];
    cout << st << endl;
    while (st[i] != '\0')
    {
        cout << st[i] << endl;
        while (st[i] != '\0' && st[i] != ',')
        {

            if (st[i] != ',')
            {
                d[move++] = st[i];
                i++;
            }
        }
        d[move] = '\0';
        //    cout<<d<<endl;
        //    cout<<stod(d)<<endl;
        double v = stod(d);
        in.setval(v);
        move = 0;
        if (st[i] == ',')
            i++;
        if (st[i] == '\0')
            break;
    }
    in.display();
    return count;
}
void *calc_h1(void *arg)
{
    hn *obj = (hn *)arg;
    double *sum = new double;
    *sum = 0;
    for (int i = 0; i < obj->fin; i++)
    {
        cout << *obj->weight << " " << obj->input[i] << endl;
        *sum += (*obj->weight) * obj->input[i];
        //        cout<<"sum:"<<*sum<<'\n';
    }
    double wr = *sum;
    double rw = wr;
    //      cout<<"Total sum:"<<*sum<<"  "<<wr<<"  "<<rw<<'\n';
    ssize_t bytes_written = write(obj->file_descriptor, &rw, sizeof(rw));
    if (bytes_written == -1)
    {
        perror("write error");
        cout << "Error code: " << errno << ", Message: " << strerror(errno) << endl;
    }

    cout << "\nBytes written : " << bytes_written << '\n';
    cout << "Total sum:" << *sum << "  " << wr << "  " << rw << '\n';
    pthread_exit(NULL);
}
string read_from_pipe()
{
    int fd = open("pipe", O_RDONLY);
                    if (fd < 0) {
                        perror("open");
                        exit(1);
                    }

                    const int BUF_SIZE = 1024;
                    char buf[BUF_SIZE];

                    ssize_t num_bytes_read;
                    const char* msg;
                    while ((num_bytes_read = read(fd, buf, BUF_SIZE)) > 0) {
                        msg = buf;
                        //std::cout << "Received message: " << msg << std::endl;
                    }
                //cout<<"\n_________________messAge is : "<<msg<<'\n';
                    if (num_bytes_read == -1) {
                        perror("read");
                        exit(1);
                    }

                    close(fd);
                    string A_str=msg;
                    A_str+='\0';
                    return A_str;
}
int main(int s, char *argv[])
 {
    cout<<"\nThe File  Running is : "<<argv[0]<<' '<<argv[1]<<'\n';
    char* a=argv[0];
    if(*a=='1')//for forwar_propagation
                                        {
            char*c=argv[1];                                
        if(*c=='e')
        {
            string A_str=read_from_pipe();
            cout<<"\nMessage : "<<A_str<<'\n';
        }
        else
        {
                    cout<<"\n________________________Ah2.cpp____________________________\n";

                        
                    int input_neurons = 2;
                    int output_neurons = 1;
                    int hidden_layers = 3;
                    int h_array[hidden_layers+1];
                    int nh_array[hidden_layers+1+1] = {0,7,7,7,1};
                    int h_temp=hidden_layers;
                    h_array[0]=0;
                    for(int i=1;i <= hidden_layers;i++)
                    {
                        h_array[i]=h_temp;
                        --h_temp;
                    }
                    int h1_neurons = 7;
                    int h2_neurons = 7;
                    int h3_neurons = 7;
                    int h4_neurons = 7;
                    int h5_neurons = 7;
                    int fd = open("pipe", O_RDONLY);
                    if (fd < 0) {
                        perror("open");
                        exit(1);
                    }

                    const int BUF_SIZE = 1024;
                    char buf[BUF_SIZE];

                    ssize_t num_bytes_read;
                    const char* msg;
                    while ((num_bytes_read = read(fd, buf, BUF_SIZE)) > 0) {
                        msg = buf;
                        std::cout << "Received message: " << msg << std::endl;
                    }
                cout<<"\n_________________messAge is : "<<msg<<'\n';
                    if (num_bytes_read == -1) {
                        perror("read");
                        exit(1);
                    }

                    close(fd);
                    string A_str=msg;
                cout<<"\n_________________messAge is : "<<A_str<<'\n';
                    int ss=0;
                    double* Arr=new double[h1_neurons];
                    int j=0;
                    int layers_on_pipe;
                    while(A_str[ss]!='*')
                    {
                        string Atemp;
                        while(A_str[ss]!=',')
                        {
                            Atemp+=A_str[ss];
                            ++ss;
                        }
                        double n=stod(Atemp);
                        Arr[j]=n;
                        ++j;
                        ++ss;
                        if(A_str[ss]=='*')
                        {
                            ++ss;
                            string At;
                        //   cout<<"come"<<'\n';
                            while(A_str[ss]!='*')
                            {
                                At+=A_str[ss];
                                ++ss;
                            }
                                layers_on_pipe=stoi(At);
                        }
                    }
                    cout<<'\n';
                    cout<<"\nHidden Layers Are : "<<layers_on_pipe<<'\n';
                    for(int i=0;i<h1_neurons;i++)
                    {
                        cout<<Arr[i]<<' ';
                    }
                    cout<<"\nNext----------------------------------------------------------->\n ";
 
                        pid_t pid = fork();
                            if (pid > 0)
                            {
                                            input h_obj;
                                            int weight_count=read("output.txt", h_obj, 0);
                                            string Ag_str;
                                            for(int i=0;i<weight_count;i++)
                                            {
                                                double rw=h_obj.inp[i]; 
                                                string temps=to_string(rw);
                                                Ag_str+=temps;
                                                Ag_str+=',';
                                            }
                                                Ag_str+='*';
                                                string Atemp=to_string(hidden_layers);
                                                Ag_str+=Atemp;
                                                Ag_str+='*';
                                                Ag_str+='\0';
                                                const char *char_array = Ag_str.c_str();
                                                cout<<"\nNew String is : "<<Ag_str<<'\n';

                                                int file_descptr = open("pipe", O_WRONLY);
                                                ssize_t bytes_written = write(file_descptr,char_array, strlen(char_array));
                                           //     cout << "\nBytes written : " << bytes_written << '\n';
                                                close(file_descptr);

                                            wait(nullptr);
                        }
                        else if (pid == 0)
                        {
                            string t=to_string(hidden_layers);
                                                        const char *char_array = t.c_str(); 
                                 execlp("./h", "0",char_array, NULL);
                        }
        }
    }
    else if(*a=='0') // for backward_propagation <<<<----
    {

    }
    pthread_exit(NULL);
    return 0;
}