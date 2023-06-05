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
//global string which  writes data on to the named pipe
string Ag_str;
double* Array_g;
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
int main(int argc,char*argv[])
{
//    cout<<"\nThe File  Running is : "<<argv[0]<<' '<<argv[1]<<'\n';
    char* a=argv[1];
        int input_neurons;
        int hidden_layers = 3;
        int h1_neurons=7;
        int h2_neurons = 7;
        int h3_neurons = 7;
        int h4_neurons = 7;
        int h5_neurons = 7;
    if(*a=='1')//for forward_propagation
                        {
    pthread_mutex_init(&m,NULL);
        
        input i_obj;
                // read from the file
                input_neurons=read("input.txt", i_obj, input_neurons);
                cout << "\ntotal:" << i_obj.total << '\n'; // can change the total here again
        input h_obj;
                h1_neurons=read("h1.txt", h_obj, h1_neurons);

        hn *send = new hn;
        send->input = new double[i_obj.total];
            for (int i = 0; i < i_obj.total; i++)
            {
                send->input[i] = i_obj.inp[i];
            }
                send->size = h_obj.total;
                send->fin = i_obj.total;
            // make a named pipe here
                mkfifo("pipe", 0666);
                    if (access("pipe", F_OK) == -1)
                    {
                        // named pipe doesn't exist
                        perror("access");
                        exit(EXIT_FAILURE);
                    }

                    if (access("pipe", R_OK) == -1)
                    {
                        // named pipe exists but isn't readable
                        perror("access");
                        exit(EXIT_FAILURE);
                    }
    pid_t pid = fork();
        if (pid > 0)
        {
//        send->file_descriptor = file_descptr;
        // now create threads for this first process and calculate the values;
        //   double* pipe_data=new double[h_obj.total];//data to be recived
            pthread_t id[h_obj.total]; // number of threads = number of input neurons
            Array_g=new double[h_obj.total];
                for (int i = 0; i < h_obj.total; i++)
                {
                    
                    void *status;
                    send->weight = new double;
                    *send->weight = h_obj.getval(i);
                        cout << "he;; " << *send->weight << endl;
                    send->file_descriptor=i;
                        pthread_create(&id[i], NULL, calc_h1, (void *)send);  
                        pthread_join(id[i], NULL);
            //  double* sum=(double*)status;
            //  cout<<"\nSum is :"<<*sum<<'\n';
                }
                     
                        Ag_str+='*';
                        string Atemp=to_string(hidden_layers);
                        Ag_str+=Atemp;
                        Ag_str+='*';
                        Ag_str+='\0';
                        const char *char_array = Ag_str.c_str();
                        cout<<"\nString is : "<<Ag_str<<'\n';

                        int file_descptr = open("pipe", O_WRONLY);
                            ssize_t bytes_written = write(file_descptr,char_array, strlen(char_array));
                            cout << "\nBytes written : " << bytes_written << '\n';
                        close(file_descptr);

                        wait(nullptr);
    }
    else if (pid == 0)
    {
                        execlp("./h", "1","3", NULL);
    }
    }
    else if(*a=='0')//for back_propagation to forward_propgation
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
                    cout<<"\nInput Layer\nNew Weights Are : ";
                    for(int i=0;i<count;i++)
                    {
                        cout<<Arr[i]<<' ';
                    }
                    cout<<'\n';
                    
                input i_obj;
                // read from the file
                read("output.txt", i_obj, count);
                cout << "\ntotal:" << i_obj.total << '\n'; // can change the total here again
        input h_obj;
                read("h1.txt", h_obj, h1_neurons);

        hn *send = new hn;
        send->input = new double[i_obj.total];
            for (int i = 0; i < i_obj.total; i++)
            {
                send->input[i] = i_obj.inp[i];
            }
                send->size = h_obj.total;
                send->fin = i_obj.total;
pthread_t id[h_obj.total]; // number of threads = number of input neurons
                for (int i = 0; i < h_obj.total; i++)
                {
                    void *status;
                    send->weight = new double;
                    *send->weight = h_obj.getval(i);
                        cout << "he;; " << *send->weight << endl;
                        pthread_create(&id[i],NULL, calc_h1, (void *)send);
                        pthread_join(id[i],NULL);
            //  double* sum=(double*)status;
            //  cout<<"\nSum is :"<<*sum<<'\n';
                }
                        Ag_str+='*';
                        string Atemp=to_string(hidden_layers);
                        Ag_str+=Atemp;
                        Ag_str+='*';
                        Ag_str+='\0';
                        const char *char_array = Ag_str.c_str();
                        cout<<"\nString is : "<<Ag_str<<'\n';

                        int file_descptr = open("pipe", O_WRONLY);
                            ssize_t bytes_written = write(file_descptr,char_array, strlen(char_array));
                            cout << "\nBytes written : " << bytes_written << '\n';
                        close(file_descptr);
                wait(nullptr);
            }
            else if(pid==0)
            {                
                cout<<"\nReached the destination, finally!";
                execlp("./h", "1","e", NULL);
            }
    }
    pthread_mutex_destroy(&m);
    pthread_exit(NULL);
    return 0;
}
