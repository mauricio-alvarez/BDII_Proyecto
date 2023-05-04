
[![](https://img.shields.io/badge/github-blue?style=for-the-badge)](https://github.com/hamzamohdzubair/redant)
[![](https://img.shields.io/badge/book-blueviolet?style=for-the-badge)](https://hamzamohdzubair.github.io/redant/)
[![](https://img.shields.io/badge/API-yellow?style=for-the-badge)](https://docs.rs/crate/redant/latest)
# **$${\color{White} BD \space II \space PROYECTO \space I }$$** 
Primer proyecto de Base de Datos II

![](database.png "Text to show on mouseover")

# **Introducción**

**Objetivo general**:
Implementar dos técnicas de organización de archivos para la rápida ejecución de consultas SQL de inserción, búsqueda y eliminación.

**Dominio de datos**:
Se han utilizado dos tablas en formato csv descargadas de la plataforma `Kaggle`.
- `credits.csv`
- `titles.csv`

**Resultado esperado**: Con la implementación las técnicas mencionadas se espera un mejor performace en la ejecución de consultas básicas SQL. 

# **Técnicas de organización de archivos**

## **SEQUENTIAL FILE**

### **Inserción**
> Para cada registro nuevo se verifica que no esten en el archivo llamando a al metodo *search()* . Se hace busqueda del maximo elemento menor en el archivo principal conplejidad *O(log(n))* y tambien en el archivo auxliar Complejidad *O(k)*. Se hace el intercambio de punteros. Y se inserta en al archivo auxliar. Complejidad O(1).

>__Note__ _**Para llamar a la función inserción:**_
```
bool add(Rtitles new_record){}
```


### **Búsqueda**
>Ya que cada registro es de lonngitud variable se opta por incorporar un archivo metadata al archivo principal con la ubicacion fisica de cada registro. De acuerdo al metadata se recorre el archivo principal aplicando el algoritmo regular de busqueda binaria. Si no se encuntra el archivo, se busca en el archivo auxiliar con busqueda lineal.

>__Note__ _**Para llamar a la función búsqueda:**_
```
Rtitles search(string key){}
```


### **Eliminación**
>Se ubica el ultimo registro registro anterior y se hace intercambio de punteros caso contrario solo se actuliza el estado del registro a -1. Es decir se aplica la estrategia del marcado.

>__Note__ _**Para llamar a la función eliminar:**_
```
bool remove(string key){}
```

### **Búsqueda por rango**
> Aprovechando el soporte de busqueda por rango de la tecnica. Se ubica el primer elemento menor en el archivo principal o axuliar y se lee los registros siguientes, siguiendo los punteros.


>__Note__ _**Para llamar a la función búsqueda por rangos:**_
```
vector<Rtitles> rangeSearch(string begin_key, string end_key){}
```


### **Recontrucción**
```
Rtitles search(string key){}
```


### **EXTENDIBLE HASH FILE**

### **Inserción**


### **Búsqueda**


### **Eliminación**


### **Experimentación**



