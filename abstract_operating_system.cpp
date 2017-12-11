#include <iostream>
#include <deque>
#include <vector>
#include <iomanip>
using namespace std;

//Process control block data structure
struct PCB
{
    int file_size;
    int time, PID_p; // unique identifier for running process
    string queue_state;
    string file; //filename
    int count;  //counter
    char RW;    //read-write
    char info;  //printer or hard drive
    int memory_size;
    
    void set_file_size(int file_size1)
    {
        file_size=file_size1;
    }
    
    string gette_file_name()
    {
        return file;
    }
    
    void set_file(string file_name)
    {
        file = file_name;
    }
    
    void set_read_write(char read_write)
    {
        RW=read_write;
    }
    
    void generate_pid(int pid)
    {
        PID_p = pid;
    }
    
    PCB()
    {
        generate_pid(count);
        count += 1;// keeping track of the number of pcb
    }
};

//Central processing unit class declaration
class CPU
{
private:
    PCB *ptr;
    bool is_cpu_occupied;
public:
    //checks if CPU is occupied
    CPU()
    {
        ptr = NULL;
        is_cpu_occupied = false;
    }
    //checks if CPU is empty
    bool CPU_isEmpty()
    {
        return !is_cpu_occupied;
    }
    // inserts a process control block to CPU
    void insert_into_cpu(PCB *objects)
    {
        if (CPU_isEmpty())
        {
            ptr = objects;
            is_cpu_occupied = true;
        }
    }
    
    PCB* getter()
    {
        return ptr;
    }
    //issuing a system call during the running section
    void system_call(char userInput_systemCall)
    {
        char read_write;
        string input;
        cout << "Input the file name: " << endl;
        cin >> input;
        ptr->set_file(input);
        //write to a printer
        if (userInput_systemCall == 'p')
            read_write = 'w';
        //read or write a drive
        else if  (userInput_systemCall == 'd')
        {
            cout << "Would you like to read or write? " << endl;
            cin >> read_write;
        }
        
        int file_size;
        //setting a read or write
        ptr->set_read_write(read_write);
        cout<<"What is the file's size? "<<endl;
        cin>>file_size;
        //setting the file size
        ptr->set_file_size(file_size);
    }
    
    void terminate()
    {
        ptr = NULL;
        is_cpu_occupied = false;
    }
};
//declaring a ready queue
deque<PCB> ready_queue;
//declaring a printer queue
vector < deque<PCB> > printer_queue;
//declaring a drive queue
vector< deque<PCB> > drive_queue;
CPU cpu_object;

void display()
{
    cout <<"Please select from the following: " << endl;
    cout <<"Press A for initiate a new process: " << endl;
    cout<<"Press p# for printer: "<<endl;
    cout<<"Press d# for drive: "<<endl;
    cout<<"Press P# to complete printer process: "<<endl;
    cout<<"Press D# to complete drive process: "<<endl;
    cout <<"Press S for a system snapshot: " << endl;
    cout <<"Press X to terminate program: " << endl;
}

int main()
{
    //System generation(sys gen)
    int printers;
    int drives;
    int memory_capacity;
    cout<<setw(29)<<"W E L C O M E"<<"\n"<<endl;
    cout << "How many Printers are in the system?" << endl;
    cin >> printers;
    for(int i=0; i<printers; i++)
    {
        deque<PCB> p;
        // add a new element to the printer queue
        printer_queue.push_back(p);
    }
    
    cout << "How many Drives are in the system?" << endl;
    cin >> drives;
    for(int i=0; i<drives; i++)
    {
        deque<PCB> d;
        //add a new element to the drive queue
        drive_queue.push_back(d);
    }
    
    cout << "What is the maximum memory in the System?" << endl;
    cin >> memory_capacity;
    //end of System generation(sys gen)
    
    char input_from_user;
    int number;
    do{
        //calling the display function
        display();
        cin>>input_from_user;
        //if user inputs X terminate the program
        if(input_from_user=='X')
        {
            exit(0);
        }
        //initiate a new process
        else if(input_from_user=='A')
        {
            PCB pcb_object;
            // add the new process to the ready queue
            ready_queue.push_back(pcb_object);
            cout<<"A process has been creadted and it's in the ready queue"<<endl;
            // checks if Cpu is ready to run a process
            if(cpu_object.CPU_isEmpty())
            {
                cpu_object.insert_into_cpu(&ready_queue.front());
                ready_queue.pop_front();
                cout<<"CPU is ready to run a process"<<endl;
            }
        }
        else if(input_from_user=='p')
        {
            //checks if CPU is empty and suggests creating a process
            if(cpu_object.CPU_isEmpty())
            {cout<<"CPU is empty process can't be completed at this time. Try creating one (A)"<<endl;}
            // else if CPU is occupied
            else
            {
                cin>>number;
                //while the printer's number is not in the range of the user's input
                while(number>printers)
                {
                    cout<<"that printer does not exist please try again"<<endl;
                    cin.clear();
                    cin.ignore(100,'\n');
                    cin>>number;
                }
                
                int index = number -1;
                cpu_object.system_call(input_from_user);
                printer_queue.at(index).push_back(*cpu_object.getter());
                
                if(!ready_queue.empty() and cpu_object.CPU_isEmpty())
                {
                    cpu_object.insert_into_cpu(&ready_queue.front());
                    ready_queue.pop_front();
                    cout<<"CPU is ready to run a process"<<endl;
                }
            }
        }
        else if(input_from_user=='d')
        {
            if(cpu_object.CPU_isEmpty())
            ////checks if CPU is empty and suggests creating a process
            {cout<<"CPU is empty process can't be completed at this time. Try creating one (A)"<<endl;}
            else
            {
                cout<<"Which drive?"<<endl;
                cin>>number;
                ///while the drive's number is not in the range of the user's input
                while(number>drives)
                {
                    cout<<"that drive does not exist please try again"<<endl;
                    cin.clear();
                    cin.ignore(100,'\n');
                    cin>>number;
                }
                int index = number -1;
                cpu_object.system_call(input_from_user);
                drive_queue.at(index).push_back(*cpu_object.getter());
                
                if(!ready_queue.empty() and cpu_object.CPU_isEmpty())
                {
                    cpu_object.insert_into_cpu(&ready_queue.front());
                    ready_queue.pop_front();
                    cout<<"CPU is ready to run a process"<<endl;
                }
            }
        }
        //printer interrupts
        else if(input_from_user=='P')
        {
            int number;
            //cout<<"which printer do you want to complete"<<endl;
            cin>> number;
            int index;
            index= number -1;
            if(printer_queue.at(index).empty()){cout<<"There is nothing to complete"<<endl;}
            else{
                ready_queue.push_back(printer_queue.at(index).front());
                printer_queue.at(index).pop_front();}
        }
        //drive interrupts
        else if(input_from_user=='D')
        {
            int number;
            cout<<"which drive do you want to complete"<<endl;
            cin>> number;
            int index;
            index= number -1;
            
            if(drive_queue.at(index).empty()){cout<<"There is nothing to complete"<<endl;}
            else{
                ready_queue.push_back(drive_queue.at(index).front());
                drive_queue.at(index).pop_front();
            }
        }
        //snapshot interrupt
        else if(input_from_user=='S')
        {
            char input;
            cin>>input;
            if(input=='r')//if 'r' shows the PIDs in the processes in the Ready Queue
            {
                int pos = 1;
                for(deque<PCB>::const_iterator i = ready_queue.begin(); i != ready_queue.end(); ++i, ++pos)
                {
                    cout<<"Ready queue elements"<< i->PID_p<<endl;
                }
            }
            else if(input=='p')
            {
                for (int i = 0; i < printer_queue.size(); ++i)
                {
                    for (deque<PCB>::const_iterator z = printer_queue[i].begin(); z != printer_queue[i].end(); ++z)
                    {
                        cout<<"Printer queue elements "<< z->PID_p<<endl;
                        cout<<"File name: "<<z->file<<endl;
                        cout<<"File size"<<z->file_size<<endl;
                    }
                }
            }
            else if(input=='d')
            {
                for (int i = 0; i < printer_queue.size(); ++i)
                {
                    for (deque<PCB>::const_iterator x = drive_queue[i].begin(); x != drive_queue[i].end(); ++x)
                    {
                        cout<<"Drive queue elements "<< x->PID_p<<endl;
                        cout<<"File name"<<x->file<<endl;
                        cout<<"File size: "<<x->file_size<<endl;
                        cout<<"type of operation: "<<x->RW<<endl;
                    }
                }
            }
        }
        else
        {
            cout<<"Invalid Input"<<endl;
        }
    }
    while(input_from_user!='X');
}
