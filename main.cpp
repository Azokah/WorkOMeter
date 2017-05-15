#include <iostream>
#include <string>
#include <ctime> // Para la fecha y hora de las tareas
#include <cstring> // para usar el strcmp
#include <vector> // Para almacenar el vector de tareas
#include <sstream> // Para usar istringstream para transformar Strings a Int

class Task{
  public:
    Task(std::string, int, std::time_t);
    ~Task();

    //Setter
    void setCompletion(int);
    //Getter
    std::string getCompletionString();
    int getCompletion();
    std::string getName();
    std::string getDate();

    std::string toString();

  private:
    std::string name;
    int completion;
    std::time_t date;
};

Task::Task(std::string NAME, int COMPLETION, std::time_t DATE){
    name = NAME;
    completion = COMPLETION;
    date = DATE;
};

void Task::setCompletion(int x){
    completion += x;
    if(completion > 100) completion = 100;
    if(completion < 0) completion = 0;
};


std::string Task::getCompletionString(){
    std::string barra;
    barra.append("[");
    for (int p = 0; p < completion / 10; p++){
        barra.append("#");
    }
    for (int p = 10; p > completion / 10; p--){
        barra.append("-");
    }
    barra.append("]");
    return barra;
};
int Task::getCompletion(){ return completion;};
    //Todo este quilombo porque asctime devuelve un ENDLINE al final del string
    //y no queria que saltara al renglon de abajo
    std::string Task::getDate() {
    std::string dateStr;
    dateStr = std::asctime(std::localtime(&date));
    dateStr.pop_back();
    return dateStr;
};
std::string Task::getName() { return name; };
std::string Task::toString(){
    //Metodo para convertir los datos de una tarea a un string y devolverlo.
    std::string str;
    str.append(getName());
    str.append(" - ");
    str.append(getCompletionString());
    str.append(". (");
    str.append(getDate());
    str.append(")");
    return str;
};

Task *newTask(std::string nameT, int completionT, std::time_t dateT){
    //Funcion que instancia una tarea y la devuelve.
    return new Task(nameT, completionT, dateT);
}

bool testArguments(int argc, char *argv[], std::vector<Task *> *vector){
    //Para poder crear tareas sin tener que ingresar a la APP. Mediante argumentos
    //Devuelve false si no habian argumentos y true si habian
    bool boolArgs = false;

    for (int args = 0; args < argc; args++){
        std::cout << argv[args] << std::endl;
    }

    if (argc == 4){
        if (std::strcmp(argv[1], "-add") == 0){
            int numero;
            std::string numeroStr(argv[3]); //transformar de Char* a String
            std::string nombreStr(argv[2]); //transformar de Char* a String
            std::istringstream (numeroStr) >> numero; //transformar de String a int
            vector->push_back(newTask(nombreStr,numero,std::time(NULL)));
            boolArgs = true;
        }
    }

    return boolArgs;
}

int main(int argc, char *argv[]){
        //Declaro las variables basicas
        int opc;
        bool running = true, choosing = true;
        std::string nameT;
        int completionT;
        std::time_t dateT;
        std::vector<Task *> tasks;
        //Checkeo si se pusieron argumentos, si no, ingreso al menu
    if (!testArguments(argc, argv,&tasks)){

        
        std::cout << "**************************" << std::endl;
        std::cout << "* Welcome to WorkOMeter! *" << std::endl;
        std::cout << "**************************" << std::endl
                  << std::endl;

        //Muestro las tareas
        for (int t = 0; t < tasks.size(); t++){
            std::cout << tasks.at(t)->toString() << std::endl;
        }

        //Entro al loop del menu
        while (running)
        {

            //Muestro las tareas
        for (int t = 0; t < tasks.size(); t++){
            std::cout << tasks.at(t)->toString() << std::endl;
        }
            choosing = true;
            while (choosing){
                std::cout << "Choose an option: " << std::endl;
                std::cout << "1. Create a task." << std::endl;
                std::cout << "2. Complete a task." << std::endl;
                std::cout << "3. Delete a task." << std::endl;
                std::cout << "4. Quit." << std::endl;
                std::cin >> opc;
                if (opc > 0 && opc < 5)
                    choosing = false;
            }

            switch (opc){
            case 1:
                //Agregar tarea al vector de tareas
                std::cout << "Insert the name of the task: ";
                std::cin >> nameT;
                std::cout << std::endl;
                std::cout << "Insert completion percentage (%): ";
                std::cin >> completionT;
                dateT = std::time(NULL);
                std::cout << std::endl;
                std::cout << "Date set as: " << std::asctime(std::localtime(&dateT)) << std::endl;
                tasks.push_back(newTask(nameT, completionT, dateT));
                break;
            case 2:
                //Completar Tarea
                std::cout << "Insert the name of the task(Exact name only): ";
                std::cin >> nameT;
                std::cout << std::endl;
                std::cout << "Insert completion percentage you want to add (%): ";
                std::cin >> completionT;
                for (int t = 0; t < tasks.size(); t++){
                    if(std::strcmp(nameT.c_str(),tasks.at(t)->getName().c_str()) == 0){
                        tasks.at(t)->setCompletion(completionT);
                    }
                }
                break;
            case 3:
                //Eliminar tarea del vector de tareas
                std::cout << "Insert the name of the task(Exact name only): ";
                std::cin >> nameT;
                for (int t = 0; t < tasks.size(); t++){
                    if(std::strcmp(nameT.c_str(),tasks.at(t)->getName().c_str()) == 0){
                        tasks.erase(tasks.begin()+t);
                    }
                }
                break;
            case 4:
                running = false;
                break;
            };
        };
    }
    return 0;
};