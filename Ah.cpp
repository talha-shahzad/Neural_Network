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
#include <semaphore.h>
#include <string>
#include <cstring>

using namespace std;

//this is the mutex to lock the threads
pthread_mutex_t m;
sem_t sem;
//global string which  writes data on to the named pipe
string Ag_str;
//this is the structure which is used to store weights or inputs
struct input
{
    public:
        double *inp;
        int total = 0;
        //functions
        input();
        input(int size);
        void setsize(int s);
        void setval(double val);
        double getval(int index);
        void display();
                                };
                                input::input()
                                    {
                                        inp = NULL;
                                        total = 0;
                                    }
                                input::input(int size)
                                    {
                                        inp = new double[size];
                                        total = 0;
                                    }    
                                void input::setsize(int s)
                                    {
                                        inp = new double[s];
                                    }
                                void input::setval(double val)
                                    {
                                        inp[total] = val;
                                        total++;
                                    }
                                double input::getval(int index)
                                    {
                                        if (index < total)
                                        {
                                            return this->inp[index];
                                        }
                                        double n=-999.9999;
                                        return n;
                                    }
                                void input::display()
                                    {
                                        for (int i = 0; i < total; i++)
                                            cout << inp[i] << " ";
                                        cout << endl;
                                    }
// hidden neuron=hd
struct hn
{
    public:
        double *input;
        double *weight;
        int size;
        int fin;
        int file_descriptor;
};

void read(string s, input &in, int neurons)
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
}
void *calc_h1(void *arg)
{
    pthread_mutex_lock(&m);
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
        string temps=to_string(rw);
        Ag_str+=temps;
        Ag_str+=',';
    //      cout<<"Total sum:"<<*sum<<"  "<<wr<<"  "<<rw<<'\n';
    // ssize_t bytes_written = write(obj->file_descriptor, &rw, sizeof(rw));
    // if (bytes_written == -1)
    // {
    //     perror("write error");
    //     cout << "Error code: " << errno << ", Message: " << strerror(errno) << endl;
    // }

    // cout << "\nBytes written : " << bytes_written << '\n';
        cout << "Total sum:" << *sum << "  " << wr << "  " << rw << '\n';
    pthread_mutex_unlock(&m);
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

                    pthread_mutex_init(&m,NULL);
 //   cout<<"\nThe File  Running is : "<<argv[0]<<' '<<argv[1]<<'\n';
    char* a=argv[0];
    if(*a=='1')//for forwar_propagation
                                        {
                    cout<<"\n________________________Ah.cpp____________________________\n";
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

                    if(layers_on_pipe<=hidden_layers)
                    {
                        int hl=h_array[layers_on_pipe];
                        cout<<"\n--------------Layer-# : "<<hl<<'\n';
                                            input i_obj;
                                            i_obj.total=nh_array[h_array[layers_on_pipe]];
                                            i_obj.inp=new double[i_obj.total];
                                            for(int i=0;i<i_obj.total;i++)
                                            {
                                                i_obj.inp[i]=Arr[i];
                                            }
                                            cout<<"\n__________________________";
                                            // read from the file
                                        //    read(".txt", i_obj, h1_neurons);
                                        //    cout << "\ntotal:" << i_obj.total << '\n'; // can change the total here again
                                            input h_obj;
                                            string weights_filename="h";
                                            weights_filename+=to_string(h_array[layers_on_pipe]+1);
                                            weights_filename+=".txt";
                                            cout<<weights_filename<<"_______________________________________\n";
                                            read(weights_filename, h_obj, nh_array[hl]);

                                            hn *send = new hn;
                                            send->input = new double[i_obj.total];
                                            for (int i = 0; i < i_obj.total; i++)
                                            {
                                                send->input[i] = i_obj.inp[i];
                                            }
                                            send->size = h_obj.total;
                                            send->fin = i_obj.total;

                                            pid_t pid = fork();
                                            if (pid > 0)
                                            {
                                            //    send->file_descriptor = file_descptr;
                                                // now create threads for this first process and calculate the values;
                                                //   double* pipe_data=new double[h_obj.total];//data to be recived
                                                pthread_t id[h_obj.total]; // number of threads = number of input neurons
                                                for (int i = 0; i < h_obj.total; i++)
                                                {
                                                    void *status;
                                                    send->weight = new double;
                                                    *send->weight = h_obj.getval(i);
                                                    cout << "he;; " << *send->weight << endl;
                                                    pthread_create(&id[i], NULL, calc_h1, (void *)send);
                                                    pthread_join(id[i], NULL);
                                                    //  double* sum=(double*)status;
                                                    //  cout<<"\nSum is :"<<*sum<<'\n';
                                                }

                                                
                                                    Ag_str+='*';
                                                    string Atemp=to_string(layers_on_pipe-1);
                                                    Ag_str+=Atemp;
                                                    Ag_str+='*';
                                                    Ag_str+='\0';
                                                    const char *char_array = Ag_str.c_str();
                                                    cout<<"\nString is : "<<Ag_str<<'\n';

                                                    int file_descptr = open("pipe", O_WRONLY);
                                                    ssize_t bytes_written = write(file_descptr,char_array, strlen(char_array));
                                                    // if (bytes_written == -1)
                                                    // {
                                                    //     perror("write error");
                                                    //     cout << "Error code: " << errno << ", Message: " << strerror(errno) << endl;
                                                    // }

                                                    cout << "\nBytes written : " << bytes_written << '\n';
                                                        close(file_descptr);

                                                wait(nullptr);

                                            }
                                            else if (pid == 0)
                                            {
                                                char* c=argv[1];
                                                cout<<"\n*************************************************************************"
                                                <<*c<<'\n';
                                                if(*c=='e')
                                                {
                                                    if(layers_on_pipe!=1)
                                                    {
                                                     execlp("./h", "1",c, NULL);
                                                    }
                                                    else
                                                    {
                                                      execlp("./h1", "1",c, NULL);
                                                    }
                                                }
                                                else
                                                {
                                                    if(layers_on_pipe!=1)
                                                    {
                                                        string t=to_string(hidden_layers);
                                                        const char *char_array = A_str.c_str(); 

                                                        execlp("./h", "1",char_array, NULL);
                                                    }
                                                    else
                                                    {
                                                        string t=to_string(hidden_layers);
                                                        const char *char_array = t.c_str(); 

                                                        execlp("./h1", "1",char_array, NULL);
                                                    }
                                                }
                                            }
                                            //
                    }
    }
    else // for backward_propagation <<<<----
    {
        char*b=argv[1];
        cout<<"\nI am in Layer# : "<<*b<<'\n';
        if(*b!='1')
        {
            char c=*b;
            int anumber=c-48;
            --anumber; 
            string str=to_string(anumber);
                int pid=fork();
                if(pid>0)
                {
                    string A_str=read_from_pipe();
                    //cout<<"\nMessage : "<<A_str<<'\n';

                    int ss=0;
                    double* Arr=new double[20];
                    int j=0;
                    int layers_on_pipe;
                    int count=0;
                    while(A_str[ss]!='*')
                    {
                        string Atemp;
                        while(A_str[ss]!=',')
                        {
                            Atemp+=A_str[ss];
                            ++ss;
                        }
                        if(A_str[ss]==','){++count;}
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
                    cout<<"\nNew Weights Are : ";
                    for(int i=0;i<count;i++)
                    {
                        cout<<Arr[i]<<' ';
                    }
                    cout<<'\n';

                    const char *char_array = A_str.c_str();                    
                    int file_descptr = open("pipe", O_WRONLY);
                                                ssize_t bytes_written = write(file_descptr,char_array, strlen(char_array));
                                           //     cout << "\nBytes written : " << bytes_written << '\n';
                                                close(file_descptr);
                    wait(nullptr);
                }
                else if(pid==0)
                {
                    execlp("./h","0",str.c_str(),NULL);// exec jadon bny c odun string hondi hi koi ni sy ty eh primitive data type use krdi a
                }
        }
        else
        {
            int pid=fork();
            if(pid>0)
            {
                string A_str=read_from_pipe();
                    //cout<<"\nMessage : "<<A_str<<'\n';

                    int ss=0;
                    double* Arr=new double[20];
                    int j=0;
                    int layers_on_pipe;
                    int count=0;
                    while(A_str[ss]!='*')
                    {
                        string Atemp;
                        while(A_str[ss]!=',')
                        {
                            Atemp+=A_str[ss];
                            ++ss;
                        }
                        if(A_str[ss]==','){++count;}
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
                    cout<<"\nNew Weights Are : ";
                    for(int i=0;i<count;i++)
                    {
                        cout<<Arr[i]<<' ';
                    }
                    cout<<'\n';
                    const char *char_array = A_str.c_str();
                    int file_descptr = open("pipe", O_WRONLY);
                                                ssize_t bytes_written = write(file_descptr,char_array, strlen(char_array));
                                           //     cout << "\nBytes written : " << bytes_written << '\n';
                                                close(file_descptr);
                    wait(nullptr);
            }
            else if(pid==0)
            {
                execlp("./input","0","0",NULL);
            }
        }
    } 

                    pthread_mutex_destroy(&m);
                    pthread_exit(NULL);
    return 0;
}