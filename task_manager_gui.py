import tkinter as tk
from tkinter import ttk, messagebox
import threading
from task_manager_bridge import (
    run_cpp_add_task,
    run_cpp_display,
    run_cpp_run_task_stream,
    run_cpp_open_log
)

# -----------------------------
# GUI Application
# -----------------------------
class TaskManagerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Task Manager Pro - GUI")
        self.root.geometry("700x500")
        self.root.configure(bg="#1e1e1e")

        title = tk.Label(
            root,
            text="TASK MANAGER PRO",
            fg="white",
            bg="#1e1e1e",
            font=("Segoe UI", 18, "bold")
        )
        title.pack(pady=10)

        # Frame for Inputs
        frame = tk.Frame(root, bg="#1e1e1e")
        frame.pack(pady=10)

        tk.Label(frame, text="Task:", fg="white", bg="#1e1e1e",
                 font=("Segoe UI", 12)).grid(row=0, column=0, sticky="w")
        self.task_entry = tk.Entry(frame, width=40, font=("Segoe UI", 11))
        self.task_entry.grid(row=0, column=1, padx=10)

        tk.Label(frame, text="Priority (0-5):", fg="white", bg="#1e1e1e",
                 font=("Segoe UI", 12)).grid(row=1, column=0, sticky="w")
        self.priority_spin = tk.Spinbox(frame, from_=0, to=5,
                                        width=5, font=("Segoe UI", 11))
        self.priority_spin.grid(row=1, column=1, sticky="w", pady=5)

        # Buttons
        btn_frame = tk.Frame(root, bg="#1e1e1e")
        btn_frame.pack(pady=10)

        style = ttk.Style()
        style.configure("TButton", font=("Segoe UI", 11), padding=5)

        self.btn_add = ttk.Button(btn_frame, text="Add Task", command=self.add_task)
        self.btn_add.grid(row=0, column=0, padx=5)
        self.btn_display = ttk.Button(btn_frame, text="Display Tasks", command=self.display_tasks)
        self.btn_display.grid(row=0, column=1, padx=5)
        self.btn_run = ttk.Button(btn_frame, text="Run Next Task", command=self.run_task)
        self.btn_run.grid(row=0, column=2, padx=5)
        self.btn_open = ttk.Button(btn_frame, text="Open Log File", command=self.open_log)
        self.btn_open.grid(row=0, column=3, padx=5)

        # Output Panel
        output_label = tk.Label(root, text="Output:", fg="white", bg="#1e1e1e",
                                font=("Segoe UI", 12))
        output_label.pack(anchor="w", padx=20)

        self.output = tk.Text(root, width=80, height=15,
                              bg="#111", fg="#0f0",
                              insertbackground="white",
                              font=("Consolas", 11))
        self.output.pack(padx=20, pady=5)

    # -----------------------------
    # Backend Action Methods
    # -----------------------------
    def add_task(self):
        task = self.task_entry.get().strip()
        priority = self.priority_spin.get()

        if not task:
            messagebox.showerror("Error", "Task cannot be empty.")
            return
        self._run_async(lambda: run_cpp_add_task(task, priority))

    def display_tasks(self):
        self._run_async(run_cpp_display)

    def run_task(self):
        # Use streaming version so 'Running....' appears immediately
        self._run_stream(run_cpp_run_task_stream)

    def open_log(self):
        self._run_async(lambda: "Opening Log...\n" + run_cpp_open_log())

    # -----------------------------
    # Output Helper
    # -----------------------------
    def write_output(self, text):
        self.output.insert(tk.END, text + "\n")
        self.output.see(tk.END)

    # -----------------------------
    # Async helpers
    # -----------------------------
    def _set_busy(self, busy: bool):
        self.root.configure(cursor="watch" if busy else "")
        for b in (self.btn_add, self.btn_display, self.btn_run, self.btn_open):
            b.state(["disabled"] if busy else ["!disabled"])

    def _run_async(self, func):
        def worker():
            try:
                result = func()
            except Exception as e:
                result = f"ERROR: {e}"
            finally:
                self.root.after(0, lambda: (self.write_output(result), self._set_busy(False)))
        self._set_busy(True)
        threading.Thread(target=worker, daemon=True).start()

    def _run_stream(self, stream_func):
        def worker():
            try:
                stream_func(lambda line: self.root.after(0, lambda: self.write_output(line)))
            finally:
                self.root.after(0, lambda: self._set_busy(False))
        self._set_busy(True)
        threading.Thread(target=worker, daemon=True).start()


# -----------------------------
# MAIN EXECUTION
# -----------------------------
if __name__ == "__main__":
    root = tk.Tk()
    app = TaskManagerGUI(root)
    root.mainloop()