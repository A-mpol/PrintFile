#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <fstream>

char ParseDelimiter(const char* optarg) {
    //ВЫЧИСЛЯЕМ ДЛИНУ СТРОКИ АРГУМЕНТА delimeter
    int len_optarg = (int) strlen(optarg);
    
    //ПО ДЛИНЕ СТРОКИ АРГУМЕНТА ОПРЕДЕЛЯЕМ СИМВОЛ РАЗДЕЛИТЕЛЯ
    switch (len_optarg) {
        case 1: return *optarg;
        case 2: {
            if (optarg[0] == '\\') {
                switch (optarg[1]) {
                    case 'a': return '\a';
                    case 'b': return '\b';
                    case 'f': return '\f';
                    case 'r': return '\r';
                    case 'v': return '\v';
                    case 'n': return '\n';
                    case 't': return '\t';
                    case '"': return '\"';
                    case '\'': return '\'';
                    case '\\': return '\\';
                    case '?': return '\?';
                    case '0': return '\0';
                }
            }
        }
    }
    
    //ВЫВОДИМ СООБЩЕНИЕ ОБ ОШИБКЕ
    std::cout << "Ошибка ввода разделителя строки. Будет использоваться символ разделителя строки по умолчанию ('\\n')";
    
    //УСТАНАВЛИВАЕМ РАЗДЕЛИТЕЛЬ ПО УМОЛЧАНИЮ
    return '\n';
}


//ПЕЧАТАЕТ НУЖНОЕ (ИЛИ ДОСТУПНОЕ) КОЛИЧЕСТВО СТРОК С КОНЦА
void PrintFromEnd(char* filename, int number_lines, char delimiter) {
    char ch;
    //ОТКРЫВАЮ ФАЙЛ, УКАЗАТЕЛЬ НА КОНЕЦ ФАЙЛА
    std::ifstream myfile(filename, std::ios_base::ate);
    
    if (!myfile.is_open()) {
        std::cerr << EXIT_FAILURE;
        return;
    }
    //ОПРЕДЕЛЯЮ КОЛИЧЕСТВО БАЙТ В ФАЙЛЕ
    std::streamoff size = myfile.tellg();
    long tmp = size;
        
    if (number_lines == -1) {
        myfile.seekg(1);
    }
    else {
        while (number_lines != 0 && size != 0) {
            myfile.seekg(size - 1);
            if (myfile.get() == delimiter && size != tmp - 1) {
                --number_lines;
            }
            --size;
        }
    }
        
    //ВЫВОЖУ СТРОКИ
    while (!myfile.eof()) {
        myfile.get(ch);
        std::cout << ch;
    }
}


void PrintFromBeginning(char* filename, int number_lines, char delimiter) {
    char ch;
    std::ifstream myfile(filename);
    
    if (!myfile.is_open()) {
        std::cerr << EXIT_FAILURE;
        return;
    }
    while (number_lines != 0 && !myfile.eof()) {
        myfile.get(ch);
        std::cout << ch;
        if (ch == delimiter) {
            --number_lines;
        }
    }
}


int main(int argc, char* argv[]) {
    int number_lines = -1;
    bool if_from_end = false;
    int rez;
    int option_index = -1;
    char delimiter = '\n';
    char* filename;
    
    // МАССИВ ДЛИННЫХ АРГУМЕНТОВ И ИХ КОРОТКИХ ВЕРСИЙ
    const char* short_options = "l:td:";
    const struct option long_options[] = {
        {"lines", required_argument, NULL, 'l'},
        {"tail", no_argument, NULL, 't'},
        {"delimiter", required_argument, NULL, 'd'},
        {NULL, 0, NULL, 0}
    };

    //ПАРСИНГ АРГУМЕНТОВ
    while ((rez=getopt_long(argc, argv, short_options,
                           long_options, &option_index)) != -1) {

        switch(rez) {
            case 't': {
                if_from_end = true;
                break;
            }
            case 'l': {
                if (optarg != NULL) {
                    number_lines = std::stoi(optarg);
                }
                
                break;
            }
            case 'd': {
                if (optarg != NULL) {
                    delimiter = ParseDelimiter(optarg);
                }
                break;
            }
            case '?': {
                std::cout << "Ошибка ввода аргументов";
                break;
            }
        }
    }
    filename = argv[optind];
    
    if (if_from_end) {
        PrintFromEnd(filename, number_lines, delimiter);
    }
    else {
        PrintFromBeginning(filename, number_lines, delimiter);
    }
    
    return EXIT_SUCCESS;
}

