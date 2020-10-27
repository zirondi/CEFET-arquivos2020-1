'''
struct Registro {
int id_inscricao; ok
char curso[20];  ok
char cpf[15]; ok 
char dataNacimento[11]; ok
char sexo; ok
char email[40]; ok 
char opcaoQuadro; ok
};
'''

import struct, random, string, datetime, numpy as np

class random_stuff():
    def __init__(self):
        #id vars
        self.id_list = [np.intc(random.randrange(0, 2147483647)) for i in range(50)]
        self.id_list.sort(reverse=True)

        #rand_string vars
        self.alphabet = string.ascii_lowercase

        #cursos vars (deve ter alguma maneira de fazer isso mais bonito)
        self.curso_list = list()
        for _i in range(5):
            temp_str = ''
            for _j in range(19):
                temp_str = temp_str + (random.choice(self.alphabet))
            self.curso_list.append(temp_str)

        #cpf vars
        self.cpf_list = [random.randrange(10000000000, 99999999999) for i in range(30)]
        self.cpf_list.sort(reverse=True)

        #date vars
        self.start_date = datetime.date(1970, 1, 1)
        self.end_date = datetime.date(2020, 10, 26)
        self.time_between =  self.end_date - self.start_date
        self.days_between = self.time_between.days

    def get_random_curso(self):
        return bytes(random.choice(self.curso_list) + '\0', encoding='utf-8')

    def format_cpf(self, cpf_long):
        cpf_string = str(cpf_long)
        cpf_final = cpf_string[0:3] + '.' + cpf_string[3:6] + '.' + cpf_string[6:9] + '-' + cpf_string[9:] + '\0' 
        return bytes(cpf_final, encoding ='utf-8')

    def get_random_date(self):
        rnd_number_of_days = random.randrange(self.days_between)
        return bytes(str(self.start_date + datetime.timedelta(days = rnd_number_of_days)) + '\0', encoding='utf-8')
    
    def get_random_sex(self):
        sex = random.choice(['m', 'f'])
        return bytes(sex, encoding='utf-8')

    def get_random_email(self):
        result_str = ''.join(random.choice(self.alphabet) for i in range(29))
        return bytes(result_str + '@email.com' + '\0', encoding = 'utf-8')
    
    def get_random_quadro(self):
        return bytes(random.choice(self.alphabet), encoding='utf-8')


candidatoStruct = struct.Struct('i20s15s11sc40sc')
random.seed('314575')
rnd = random_stuff()
ids = rnd.id_list
cpfs = rnd.cpf_list

f = open('./candidatos.dat', 'wb')

count = 0
for i in range(0,30):
    if count % 5 == 0:
        #Pegando a primeira mencao do cpf na lista
        candidate = [ids.pop(), 
        rnd.get_random_curso(), 
        rnd.format_cpf(cpfs.pop()), 
        rnd.get_random_date(), 
        rnd.get_random_sex(), 
        rnd.get_random_email(),  
        rnd.get_random_quadro()]

        f.write(candidatoStruct.pack(*candidate))

        #Pegando a segunda mençao do cpf na lista
        index = random.randrange(0, len(ids))
        candidate_duplicate = [ids.pop(index)]
        candidate_duplicate.extend(candidate[1:])

        f.write(candidatoStruct.pack(*candidate_duplicate))

        count += 2
    else:
        candidate = [ids.pop(), 
        rnd.get_random_curso(), 
        rnd.format_cpf(cpfs.pop()), 
        rnd.get_random_date(), 
        rnd.get_random_sex(), 
        rnd.get_random_email(),  
        rnd.get_random_quadro()]

        f.write(candidatoStruct.pack(*candidate))
        count += 1


f.close()
