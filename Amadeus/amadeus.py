import tkinter as tk
# Import the file where your IDE class is saved (assuming it is named custom_languageIDE.py)
import custom_languageIDE 

def main():
    root = tk.Tk()
    
    # FIX: Access the class 'CustomLanguageIDE' inside the module 'custom_languageIDE'
    app = custom_languageIDE.CustomLanguageIDE(root)
    
    root.mainloop()

if __name__ == "__main__":
    main()