# **xv6 Threads & Synchronization – Extended Kernel (Parts A–D)**

This project extends the xv6 operating system (x86 version) by implementing:

✔ A new system call `waitpid()`
✔ Kernel-level barrier synchronization
✔ A full user-level threading library
✔ User-space spinlocks using atomic instructions

This README explains setup, installation, modified files, and how to run each part’s testcases.

---

## ⭐ **Features Implemented**

### 🔹 **Part A — `waitpid()` System Call**

Adds a new syscall to reap a *specific* child process by PID.

### 🔹 **Part B — Kernel Barrier Synchronization**

Implements blocking barriers using kernel `sleep()` / `wakeup()`.

### 🔹 **Part C — User-Level Threads**

Adds:

* `thread_create()`
* `thread_exit()`
* `thread_join()`

Threads share the address space and file descriptors but have individual:

* kernel stacks
* user stacks
* trapframes

### 🔹 **Part D — User-Space Spinlocks**

Implements spinlocks entirely in userspace using the atomic `xchg` instruction.

---

# **📦 1. Prerequisites**

Install required packages:

```bash
sudo apt-get update
sudo apt-get install build-essential gcc make python3 python3-pip git
```

---

# **🚀 2. Install QEMU**

xv6 requires QEMU to run.

```bash
git clone https://gitlab.com/qemu-project/qemu.git
cd qemu
./configure
make
```

If you get Sphinx errors:

```bash
pip install sphinx==6.2.1
pip install sphinx-rtd-theme==1.2.2
```

If ninja is missing:

```bash
sudo apt-get install ninja-build
```

---

# **🛠️ 3. Download xv6 Starter Code**

Use the patched xv6 code provided in the OS lab:

```
https://www.cse.iitb.ac.in/~mythili/os/labs/lab-xv6-threads-sync/xv6-threads-sync-code.tgz
```

Extract:

```bash
tar -zxvf xv6-threads-sync-code.tgz
cd xv6-public
```

---

# **🧬 4. Apply Modified Files**

This project modifies the following xv6 source files:

| File        | Purpose                                    |
| ----------- | ------------------------------------------ |
| `proc.c`    | waitpid + threads logic                    |
| `proc.h`    | new fields (is_thread, mainthread, ustack) |
| `defs.h`    | added kernel function prototypes           |
| `sysproc.c` | sys_waitpid + sys_thread_*                 |
| `ulib.c`    | user-space spinlocks + wrappers            |
| `user.h`    | user API for threads + locks               |
| `user.c`    | stub functions for syscalls                |
| `barrier.c` | kernel barrier implementation              |

Replace these files directly in `xv6-public/`:


# **🏗️ 5. Build xv6**

Inside `xv6-public`:

```bash
make clean
make
```

When successful, kernel and user programs compile without errors.

---

# **💻 6. Run xv6 in QEMU**

GUI mode:

```bash
make qemu
```

Terminal-only mode:

```bash
make qemu-nox
```

If successful, you will see:

```
init: starting sh
$
```

---

# **🧪 7. Running Tests**

Four tests are provided:

| Test Program | Description  |
| ------------ | ------------ |
| `t_waitpid`  | tests Part A |
| `t_barrier`  | tests Part B |
| `t_threads`  | tests Part C |
| `t_lock`     | tests Part D |

Run any test inside xv6:

```bash
$ t_waitpid
$ t_barrier
$ t_threads
$ t_lock
```

---

# **📊 8. Expected Outputs**

### ✔ **waitpid**

```
return value of wrong waitpid -1
return value of correct waitpid <pid>
return value of wait -1
child reaped
```

### ✔ **barrier**

Processes block until all arrive; order changes accordingly.

### ✔ **threads**

```
Hello World
Thread 1 created 10
Thread 2 created 10
Value of x = 11
```

### ✔ **spinlocks**

```
Final value of x: 2000000
```

---

# **🧠 9. Architecture Summary**

### **Part A — waitpid**

* Kernel scans ptable for specific child PID
* Reaps only that child
* Blocks until ZOMBIE or returns -1

### **Part B — Barrier**

* Tracks arrival count using spinlock
* Sleeps early arrivals
* Last arrival wakes up all waiters

### **Part C — Threads**

* `thread_create()` allocates new `proc` but shares parent’s page table
* Allocates unique user stack
* Sets trapframe `eip` and `esp`
* `thread_join()` reaps only thread resources

### **Part D — Spinlocks**

* Pure user-space mutual exclusion
* Built using atomic `xchg` instruction
* Protects shared memory from concurrent access

---

# **📌 10. Adding New Programs**

To add a new user program:

1. Create `progname.c` in `xv6-public/`
2. Add to Makefile:

```make
UPROGS += _progname
```

3. Rebuild:

```bash
make clean
make
```

Run inside xv6:

```bash
$ progname
```

---

#  11. Troubleshooting

### QEMU not found?

```bash
export PATH=$PATH:/home/<user>/qemu/build/
```

### Panic: acquire?

Usually caused by:

* freeing stacks incorrectly
* incorrect thread cleanup
* wrong sleep/wakeup channel

Check your `thread_exit()` and `thread_join()`.

### Unexpected `copyout` failures?

Check:

* stack alignment in `thread_create()`
* `PGROUNDUP` of `sz`
* correct stack pointer arithmetic

---

# **👨‍💻 Author**

This repository contains an extended version of xv6 implementing advanced OS concepts:

* system calls
* synchronization
* threads
* user-space concurrency primitives

Ideal for OS lab assignments and educational use.

---

