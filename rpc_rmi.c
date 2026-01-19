# Calculatrice RMI & RPC - Guide Complet

## 1️⃣ RPC en C – Calculatrice Simple

### Fichier `calc.x`

```c
struct num {
    int a;
    int b;
};

program CALC_PROG {
    version CALC_VER1 {
        int ADD(num) = 1;
        int MULT(num) = 2;
    } = 1;
} = 0x20000001;
```

### Génération automatique

```bash
rpcgen -a calc.x
```

> Cela génère : `calc.h`, `calc_clnt.c`, `calc_svc.c`, `calc_xdr.c`, `calc_server.c`, `calc_client.c`, Makefile

### Server – `calc_server.c`

```c
#include "calc.h"

int *add_1_svc(num *n, struct svc_req *rqstp) {
    static int result;
    result = n->a + n->b;
    return &result;
}

int *mult_1_svc(num *n, struct svc_req *rqstp) {
    static int result;
    result = n->a * n->b;
    return &result;
}
```

### Client – `calc_client.c`

```c
#include <stdio.h>
#include "calc.h"

int main() {
    CLIENT *client;
    struct num n;
    int *res;

    client = clnt_create("localhost", CALC_PROG, CALC_VER1, "udp");
    if (client == NULL) {
        perror("Erreur création client");
        return 1;
    }

    n.a = 4;
    n.b = 3;

    res = add_1(&n, client);
    printf("Addition: %d + %d = %d\n", n.a, n.b, *res);

    res = mult_1(&n, client);
    printf("Multiplication: %d * %d = %d\n", n.a, n.b, *res);

    return 0;
}
```

### Compilation et exécution RPC C

```bash
gcc -o server calc_server.c calc_svc.c calc_xdr.c -lnsl
gcc -o client calc_client.c calc_clnt.c calc_xdr.c -lnsl
./server  # lance le serveur
./client  # lance le client
```

---

## 2️⃣ RMI en Java – Calculatrice Simple

### Interface – `Math.java`

```java
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Math extends Remote {
    int add(int a, int b) throws RemoteException;
    int sub(int a, int b) throws RemoteException;
}
```

### Implémentation – `MathImpl.java`

```java
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class MathImpl extends UnicastRemoteObject implements Math {
    public MathImpl() throws RemoteException {
        super();
    }

    public int add(int a, int b) throws RemoteException {
        return a + b;
    }

    public int sub(int a, int b) throws RemoteException {
        return a - b;
    }
}
```

### Serveur – `MathServer.java`

```java
import java.rmi.Naming;

public class MathServer {
    public static void main(String[] args) {
        try {
            MathImpl obj = new MathImpl();
            Naming.rebind("rmi://localhost/math", obj);
            System.out.println("Serveur RMI lancé !");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```

### Client – `MathClient.java`

```java
import java.rmi.Naming;

public class MathClient {
    public static void main(String[] args) {
        try {
            Math obj = (Math) Naming.lookup("rmi://localhost/math");
            System.out.println("5 + 3 = " + obj.add(5, 3));
            System.out.println("5 - 3 = " + obj.sub(5, 3));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```

### Compilation et exécution RMI

```bash
javac *.java
rmiregistry &
java MathServer
java MathClient
```

---

## 3️⃣ Tableau comparatif RMI vs RPC

| Composant                | RMI (Java)                                         | RPC (C)                                       |
| ------------------------ | -------------------------------------------------- | --------------------------------------------- |
| Objet distant / fonction | Objet distant (interface + impl)                   | Fonction distante (`_svc`)                    |
| Stub côté client         | Emballe arguments, envoie, reçoit résultat         | Emballe arguments, envoie, récupère résultat  |
| Skeleton côté serveur    | Déballe, exécute méthode, emballe résultat         | Déballe, exécute fonction, emballe résultat   |
| Passage des données      | Objets locaux → copie, objets distants → référence | Structs → pointeur, résultat → static         |
| Exception / erreur       | RemoteException si serveur inaccessible            | Retour NULL ou erreur si serveur inaccessible |
| Enregistrement           | `rmiregistry`                                      | rpcgen génère automatiquement stub/squelette  |
| Communication            | TCP/IP (RMI)                                       | TCP ou UDP                                    |

> Ce tableau est très pratique pour réviser avant l’examen et comparer les deux concepts.

---

**Fin du README – Révision complète RMI + RPC**
