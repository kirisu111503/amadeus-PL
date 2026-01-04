import tkinter as tk
from tkinter import ttk, messagebox, filedialog, simpledialog, font
import os
import subprocess
import shutil
import threading 
import lexer  

# AESTHETIC: Soft "Bioluminescent" Futuristic Minimalist
COLORS = {
    'bg_main': '#121b17',       # Editor / Main background
    'bg_sec': '#1a2621',        # Sidebar / Panels
    'fg': '#d0e8d8',            # Main text color
    'fg_dim': '#6b8e7c',        # Dim text / Line numbers
    'accent': '#4db68d',        # Main accent color
    'accent_hover': '#5fcba1',  # Slightly brighter glow
    'accent_fg': '#121b17',     # Text on accent buttons
    'tab_bg': '#1a2621',        # Inactive tab background
    'tab_fg': '#6b8e7c',        # Inactive tab text
    'tab_active_bg': '#121b17', # Active tab background
    'tab_active_fg': '#4db68d', # Active tab text
    'select_bg': '#2a453b',     # Selection layer
    'success': '#81c784',       # Success green
    'error': '#e57373'          # Alert red
}

class CustomLanguageIDE:
    def __init__(self, root):
        self.root = root
        self.root.title("Amadeus") 
        self.root.geometry("1200x800")
        self.root.configure(bg=COLORS['bg_main'])
        
        # Set application icon
        try:
            self.root.iconbitmap('ico.ico')
        except Exception:
            try:
                icon = tk.PhotoImage(file='ico.png')
                self.root.iconphoto(True, icon)
            except Exception as e:
                print(f"Could not load icon: {e}")
        
        self.expanded_paths = set()

        # --- NEW DEFAULT FOLDER LOGIC ---
        # 1. Define the path for "untitled" relative to where the .exe/script is running
        base_path = os.getcwd() 
        default_folder = os.path.join(base_path, "untitled")
        
        # 2. Check if it exists, if not, create it
        if not os.path.exists(default_folder):
            try:
                os.makedirs(default_folder)
            except OSError as e:
                print(f"Could not create default folder: {e}")
                default_folder = base_path # Fallback to main folder if permission denied
        
        # 3. Set this as the current directory
        self.current_directory = default_folder
        # -------------------------------
        
        self.word_wrap_active = False 
        
        # --- FONT ENGINE ---
        self.base_font_size = 11
        self.editor_font = font.Font(family="Consolas", size=self.base_font_size)
        
        # --- STYLE ENGINE ---
        self.setup_styles()
        
        # --- MAIN LAYOUT ---
        main_frame = tk.Frame(root, bg=COLORS['bg_main'])
        main_frame.pack(fill=tk.BOTH, expand=True, padx=8, pady=8)

        # Main Split Container
        self.main_container = ttk.PanedWindow(main_frame, orient=tk.HORIZONTAL)
        self.main_container.pack(fill=tk.BOTH, expand=True)
        
        # Left Panel (File Explorer)
        self.create_file_explorer(self.main_container)
        
        # Right Panel (Editor + Output)
        self.right_panel = ttk.PanedWindow(self.main_container, orient=tk.VERTICAL)
        self.main_container.add(self.right_panel, weight=4)
        
        self.create_editor_section(self.right_panel)
        self.create_output_section(self.right_panel)
        
        # Menus and Shortcuts
        self.create_menu_bar()
        self.bind_shortcuts()
        
        # Startup routines
        self.close_all_tabs()
        self.refresh_file_tree()
        self.start_live_monitoring()
        
        # Optional: Print to console that we opened the default folder
        self.output_text.insert('end', f"📁 Default workspace loaded: {os.path.basename(self.current_directory)}\n", 'info')

    def setup_styles(self):
        self.style = ttk.Style()
        self.style.theme_use('clam')
        
        # Base Configuration
        self.style.configure('.', 
            background=COLORS['bg_sec'], 
            foreground=COLORS['fg'], 
            font=('Segoe UI', 10),
            borderwidth=0,
            relief='flat'
        )
        
        # PanedWindow & Treeview
        self.style.configure("TPanedwindow", background=COLORS['bg_main'], sashwidth=8, sashrelief='flat')
        
        self.style.configure("Treeview", 
            background=COLORS['bg_sec'],
            foreground=COLORS['fg'],
            fieldbackground=COLORS['bg_sec'],
            borderwidth=0,
            font=('Segoe UI', 11),
            rowheight=28
        )
        self.style.map("Treeview", 
            background=[('selected', COLORS['select_bg'])],
            foreground=[('selected', COLORS['accent'])]
        )
        self.style.configure("Treeview.Heading", 
            background=COLORS['bg_sec'], 
            foreground=COLORS['fg_dim'], 
            borderwidth=0,
            font=('Segoe UI', 9, 'bold')
        )

        # Tab Close Button Image Setup
        try:
            self.close_img = tk.PhotoImage(file="close.png")
        except Exception:
            self.close_img = tk.PhotoImage(width=1, height=1)

        try:
            self.style.element_create("close", "image", self.close_img,
                ("active", self.close_img), border=0, sticky='')
        except tk.TclError:
            pass 

        # Notebook Layout
        self.style.layout("General.TNotebook.Tab", [
            ("General.TNotebook.tab", {
                "sticky": "nswe", 
                "children": [
                    ("General.TNotebook.padding", {
                        "side": "top", 
                        "sticky": "nswe",
                        "children": [
                            ("General.TNotebook.focus", {
                                "side": "top", 
                                "sticky": "nswe",
                                "children": [
                                    ("General.TNotebook.label", {"side": "left", "sticky": "ns"}),
                                    ("close", {"side": "left", "sticky": ""}), 
                                ]
                            })
                        ]
                    })
                ]
            })
        ])
        
        # Tab Styling
        self.style.configure("General.TNotebook", background=COLORS['bg_main'], borderwidth=0)
        self.style.configure("General.TNotebook.Tab", 
            background=COLORS['tab_bg'], 
            foreground=COLORS['tab_fg'],
            font=('Segoe UI', 10),
            borderwidth=0,
            padding=[15, 10] 
        )
        self.style.map("General.TNotebook.Tab", 
            background=[("selected", COLORS['tab_active_bg'])],
            foreground=[("selected", COLORS['tab_active_fg'])]
        )
        
        # Button Styling
        self.style.configure("Accent.TButton", 
            font=('Segoe UI', 9, 'bold'),
            background=COLORS['accent'], 
            foreground=COLORS['accent_fg'], 
            borderwidth=0,
            focusthickness=0,
            padding=[12, 6]
        )
        self.style.map("Accent.TButton", 
            background=[("active", COLORS['accent_hover'])],
            foreground=[("active", COLORS['accent_fg'])]
        )
        
        self.style.configure("Toggle.TButton", 
            font=('Segoe UI', 9, 'bold'),
            background=COLORS['bg_sec'], 
            foreground=COLORS['fg_dim'], 
            borderwidth=0,
            focusthickness=0,
            padding=[12, 6]
        )

        self.style.configure("Icon.TButton",
            background=COLORS['bg_sec'],
            foreground=COLORS['fg_dim'],
            borderwidth=0,
            focusthickness=0,
            font=('Segoe UI', 12),
            padding=4
        )
        self.style.map("Icon.TButton", foreground=[("active", COLORS['accent'])])

        # --- SCROLLBAR STYLING ---
        self.style.configure("TScrollbar",
            gripcount=0,
            borderwidth=0,
            relief='flat',
            background=COLORS['fg_dim'],      
            troughcolor=COLORS['bg_sec'],     
            darkcolor=COLORS['bg_sec'],       
            lightcolor=COLORS['bg_sec'],      
            arrowcolor=COLORS['fg'],          
            arrowsize=16                      
        )

        self.style.map("TScrollbar",
            background=[('pressed', COLORS['accent_hover']), ('active', COLORS['accent'])],
            arrowcolor=[('pressed', COLORS['accent_hover']), ('active', COLORS['accent'])]
        )
        
    def run_syntax_highlighting(self, tab_frame):
        if not hasattr(tab_frame, 'editor') or not tab_frame.winfo_exists():
            return
            
        editor = tab_frame.editor
        content = editor.get("1.0", "end-1c")
        
        # 1. Clear existing tags (Reset to default)
        for tag in editor.tag_names():
            if tag != "sel": 
                editor.tag_remove(tag, "1.0", "end")
        
        # 2. LEXER PASS (Using imported lexer module)
        try:
            lex = lexer.Lexer(content) 
            tokens = lex.tokenize()
        except Exception as e:
            print(f"Lexer Error: {e}")
            return
        
        # 3. RENDER PASS
        configured_tags = set(editor.tag_names())

        for token in tokens:
            start_index = f"{token.line}.{token.column}"
            end_index = f"{token.line}.{token.column + len(token.value)}"
            
            if token.type.startswith("KEYWORD_"):
                tag_name = token.type
                
                if tag_name not in configured_tags:
                    unique_color = lexer.generate_color(token.value)
                    editor.tag_configure(tag_name, 
                                        foreground=unique_color, 
                                        font=(self.editor_font['family'], self.editor_font['size'], 'bold'))
                    configured_tags.add(tag_name)
                
                editor.tag_add(tag_name, start_index, end_index)
                
            elif token.type in ["STRING", "COMMENT", "NUMBER", "CHAR"]:
                editor.tag_add(token.type, start_index, end_index)
                
            elif token.type == "MISMATCH":
                editor.tag_add("ERROR", start_index, end_index)
                editor.tag_configure("ERROR", background="#ff0000", foreground="white")

    def bind_shortcuts(self):
        self.root.bind("<Control-n>", lambda e: self.new_file())
        self.root.bind("<Control-o>", lambda e: self.open_file())
        self.root.bind("<Control-Shift-O>", lambda e: self.open_folder())
        self.root.bind("<Control-s>", lambda e: self.save_file())
        self.root.bind("<F5>", lambda e: self.compile_and_run())
        self.root.bind("<F2>", lambda e: self.rename_current_tab())
        self.root.bind("<Control-w>", lambda e: self.close_tab())
        self.root.bind("<Control-Tab>", lambda e: self.cycle_tabs(1))
        self.root.bind("<Control-Shift-Tab>", lambda e: self.cycle_tabs(-1))
        self.root.bind("<Control-plus>", lambda e: self.zoom_in())
        self.root.bind("<Control-equal>", lambda e: self.zoom_in())
        self.root.bind("<Control-minus>", lambda e: self.zoom_out())
        self.root.bind("<Control-MouseWheel>", self.on_mouse_zoom)
        self.root.bind("<Alt-z>", lambda e: self.toggle_word_wrap())

    def zoom_in(self):
        # 1. Calculate new size
        current_size = self.editor_font.cget("size")
        new_size = min(current_size + 1, 30) # Increased max limit slightly
        
        # 2. Update the main font object (Updates normal text automatically)
        self.editor_font.configure(size=new_size)
        
        # 3. Manually update the KEYWORDS (bold text)
        self.refresh_ui_font(new_size)

    def zoom_out(self):
        # 1. Calculate new size
        current_size = self.editor_font.cget("size")
        new_size = max(current_size - 1, 8) 
        
        # 2. Update the main font object
        self.editor_font.configure(size=new_size)
        
        # 3. Manually update the KEYWORDS
        self.refresh_ui_font(new_size)
    
    def refresh_ui_font(self, new_size):
        current_family = self.editor_font.cget("family")
        
        # Iterate over every open tab
        for tab_id in self.notebook.tabs():
            tab_frame = self.notebook.nametowidget(tab_id)
            if hasattr(tab_frame, 'editor'):
                editor = tab_frame.editor
                
                # A. Update Line Numbers
                tab_frame.line_numbers.configure(font=self.editor_font)
                
                # B. CRITICAL FIX: Update the font for every Keyword tag
                # We search for tags that look like "KEYWORD_INT", "KEYWORD_CHAR", etc.
                for tag in editor.tag_names():
                    if tag.startswith("KEYWORD_"):
                        # We re-apply the font with the NEW size and KEEP it bold
                        editor.tag_configure(tag, font=(current_family, new_size, 'bold'))
                
                # C. Force a redraw of line numbers
                self.root.after(10, lambda tf=tab_frame: self.update_line_numbers(tf))

    def on_mouse_zoom(self, event):
        # FIX FOR "OPPOSITE" DIRECTION:
        # If scrolling UP (Zoom In) makes it smaller, swap these two lines.
        # Windows standard: delta > 0 is UP.
        
        if event.delta > 0:
            self.zoom_in()  # Scroll UP -> Text gets BIGGER
        else:
            self.zoom_out() # Scroll DOWN -> Text gets SMALLER

    def create_menu_bar(self):
        menubar = tk.Menu(self.root, bg=COLORS['bg_sec'], fg=COLORS['fg'], bd=0, activebackground=COLORS['select_bg'], activeforeground=COLORS['accent'])
        self.root.config(menu=menubar)
        
        def make_menu(label):
            m = tk.Menu(menubar, tearoff=0, bg=COLORS['bg_sec'], fg=COLORS['fg'], 
                        activebackground=COLORS['select_bg'], activeforeground=COLORS['accent'], bd=0)
            menubar.add_cascade(label=label, menu=m)
            return m

        file_menu = make_menu("File")
        file_menu.add_command(label="New File (Ctrl+N)", command=self.new_file)
        file_menu.add_command(label="Open File (Ctrl+O)", command=self.open_file)
        file_menu.add_command(label="Open Folder (Ctrl+Shift+O)", command=self.open_folder)
        file_menu.add_separator()
        file_menu.add_command(label="Save (Ctrl+S)", command=self.save_file)
        file_menu.add_command(label="Save As", command=self.save_file_as)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self.root.quit)
        
        edit_menu = make_menu("Edit")
        edit_menu.add_command(label="Rename File (F2)", command=self.rename_current_tab)
        edit_menu.add_separator()
        edit_menu.add_command(label="Toggle Word Wrap (Alt+Z)", command=self.toggle_word_wrap)
        edit_menu.add_separator()
        edit_menu.add_command(label="Zoom In (Ctrl+)", command=self.zoom_in)
        edit_menu.add_command(label="Zoom Out (Ctrl-)", command=self.zoom_out)
        
        run_menu = make_menu("Run")
        run_menu.add_command(label="Compile & Run (F5)", command=self.compile_and_run)

    def create_file_explorer(self, parent):
        file_frame = tk.Frame(parent, bg=COLORS['bg_sec'])
        parent.add(file_frame, weight=1)
        
        header_frame = tk.Frame(file_frame, bg=COLORS['bg_sec'])
        header_frame.pack(fill=tk.X, padx=15, pady=15)
        
        tk.Label(header_frame, text="PROJECT", bg=COLORS['bg_sec'], fg=COLORS['fg_dim'], 
                 font=('Segoe UI', 9, 'bold')).pack(side=tk.LEFT)
        
        ttk.Button(header_frame, text="↻", style="Icon.TButton", 
                   command=self.refresh_file_tree).pack(side=tk.RIGHT)
        
        tree_frame = tk.Frame(file_frame, bg=COLORS['bg_sec'])
        tree_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        self.file_tree = ttk.Treeview(tree_frame, selectmode='browse', show='tree')
        self.file_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        scrollbar = ttk.Scrollbar(tree_frame, orient=tk.VERTICAL, command=self.file_tree.yview)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.file_tree.configure(yscrollcommand=scrollbar.set)
        
        self.file_tree.bind('<Double-1>', self.on_file_double_click)
        self.file_tree.bind('<Button-3>', self.show_context_menu)
        self.file_tree.bind('<<TreeviewOpen>>', self.on_tree_open)
        self.file_tree.bind('<<TreeviewClose>>', self.on_tree_close)

    def create_editor_section(self, parent):
        editor_outer = tk.Frame(parent, bg=COLORS['bg_main'])
        parent.add(editor_outer, weight=3)
        
        toolbar = tk.Frame(editor_outer, bg=COLORS['bg_main'], height=50)
        toolbar.pack(fill=tk.X, padx=10, pady=(10, 0))
        toolbar.pack_propagate(False)
        
        self.notebook = ttk.Notebook(editor_outer, style="General.TNotebook")
        self.notebook.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        # --- BINDINGS FOR TABS ---
        self.notebook.bind('<ButtonRelease-1>', self.on_tab_click)
        self.notebook.bind('<Button-3>', self.show_tab_context_menu)
        # NEW: Middle Click to Close
        self.notebook.bind('<Button-2>', self.on_tab_middle_click) 
        
        btn_frame = tk.Frame(toolbar, bg=COLORS['bg_main'])
        btn_frame.pack(side=tk.RIGHT, pady=5)
        
        ttk.Button(btn_frame, text="RUN ▶", style="Accent.TButton", 
                   command=self.compile_and_run).pack(side=tk.RIGHT, padx=5)
        
        ttk.Button(btn_frame, text="SAVE", style="Accent.TButton", 
                   command=self.save_file).pack(side=tk.RIGHT, padx=5)

        self.wrap_btn = ttk.Button(btn_frame, text="WRAP ↩", style="Toggle.TButton", 
                   command=self.toggle_word_wrap)
        self.wrap_btn.pack(side=tk.RIGHT, padx=5)
        
        # Don't create default tab - let user open/create files

    def toggle_word_wrap(self):
        self.word_wrap_active = not self.word_wrap_active
        wrap_mode = 'word' if self.word_wrap_active else 'none'
        
        if self.word_wrap_active:
            self.style.configure("Toggle.TButton", background=COLORS['select_bg'], foreground=COLORS['accent'])
        else:
            self.style.configure("Toggle.TButton", background=COLORS['bg_sec'], foreground=COLORS['fg_dim'])
            
        for tab_id in self.notebook.tabs():
            tab_frame = self.notebook.nametowidget(tab_id)
            if hasattr(tab_frame, 'editor'):
                tab_frame.editor.config(wrap=wrap_mode)
                self.root.after(100, lambda tf=tab_frame: self.update_line_numbers(tf))

    def create_new_editor_tab(self, filename):
        tab_frame = tk.Frame(self.notebook, bg=COLORS['bg_main'])
        
        text_container = tk.Frame(tab_frame, bg=COLORS['bg_main'], padx=5, pady=5)
        text_container.pack(fill=tk.BOTH, expand=True)
        
        # --- Line Numbers ---
        line_numbers = tk.Text(text_container, width=4, padx=10, takefocus=0, border=0,
                               bg=COLORS['bg_main'], fg=COLORS['fg_dim'], 
                               state='disabled', wrap='none', font=self.editor_font)
        line_numbers.pack(side=tk.LEFT, fill=tk.Y)
        
        # --- Main Editor ---
        initial_wrap = 'word' if self.word_wrap_active else 'none'
        editor = tk.Text(text_container, wrap=initial_wrap, undo=True, 
                         font=self.editor_font,
                         bg=COLORS['bg_main'], 
                         fg=COLORS['fg'],
                         insertbackground=COLORS['accent'],
                         insertwidth=2,
                         selectbackground=COLORS['select_bg'],
                         bd=0, padx=10)
        editor.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        # --- Scrollbars ---
        v_scrollbar = ttk.Scrollbar(text_container, orient=tk.VERTICAL, command=editor.yview)
        v_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        h_scrollbar = ttk.Scrollbar(tab_frame, orient=tk.HORIZONTAL, command=editor.xview)
        h_scrollbar.pack(side=tk.BOTTOM, fill=tk.X)
        
        def sync_y_scroll(*args):
            v_scrollbar.set(*args)
            line_numbers.yview_moveto(args[0])
            
        editor.configure(yscrollcommand=sync_y_scroll, xscrollcommand=h_scrollbar.set)
        line_numbers.configure(yscrollcommand=lambda *args: None)
        
        # --- State Management ---
        tab_frame.editor = editor
        tab_frame.line_numbers = line_numbers
        tab_frame.filename = filename
        tab_frame.filepath = None
        tab_frame._line_update_job = None
        tab_frame._highlight_job = None
        
        # --- SYNTAX HIGHLIGHTING SETUP ---
        editor.tag_configure("STRING", foreground="#e6db74") 
        editor.tag_configure("CHAR", foreground="#e6db74")  
        editor.tag_configure("COMMENT", foreground="#75715e") 
        editor.tag_configure("NUMBER", foreground="#ae81ff") 
        
        self.notebook.add(tab_frame, text=f"{filename} ")
        self.notebook.select(tab_frame)
        
        # --- EVENT BINDINGS ---
        
        def trigger_update(event=None):
            self.update_line_numbers(tab_frame)
            
            if hasattr(tab_frame, '_highlight_job') and tab_frame._highlight_job:
                self.root.after_cancel(tab_frame._highlight_job)
                
            tab_frame._highlight_job = self.root.after(15, lambda: self.run_syntax_highlighting(tab_frame))

            if event and event.type == '2': 
                editor.see('insert')

        editor.bind('<<Modified>>', lambda e: [trigger_update(e), editor.edit_modified(False)])
        editor.bind('<Configure>', trigger_update)
        editor.bind("<Control-MouseWheel>", self.on_mouse_zoom)
        
        self.root.after(50, lambda: trigger_update())
        
        return tab_frame

    def create_output_section(self, parent):
        output_frame = tk.Frame(parent, bg=COLORS['bg_sec'])
        parent.add(output_frame, weight=1)
        
        header = tk.Frame(output_frame, bg=COLORS['bg_sec'])
        header.pack(fill=tk.X, padx=15, pady=10)
        tk.Label(header, text="SYSTEM OUTPUT", bg=COLORS['bg_sec'], fg=COLORS['fg_dim'], 
                 font=('Segoe UI', 9, 'bold')).pack(side=tk.LEFT)
        
        ttk.Button(header, text="Clear", style="Icon.TButton", 
                   command=self.clear_output).pack(side=tk.RIGHT)
        
        output_container = tk.Frame(output_frame, bg=COLORS['bg_main'])
        output_container.pack(fill=tk.BOTH, expand=True, padx=10, pady=(0,10))
        
        self.output_text = tk.Text(output_container, height=8, wrap='word', 
                                   font=('Consolas', 10), 
                                   bg=COLORS['bg_main'], 
                                   fg=COLORS['fg'],
                                   bd=0, padx=10, pady=10,
                                   insertbackground=COLORS['accent'],
                                   selectbackground=COLORS['select_bg'])
        self.output_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        self.output_text.tag_config('success', foreground=COLORS['success'])
        self.output_text.tag_config('error', foreground=COLORS['error'])
        self.output_text.tag_config('info', foreground=COLORS['accent'])
        
        scroll = ttk.Scrollbar(output_container, orient=tk.VERTICAL, command=self.output_text.yview)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.output_text.configure(yscrollcommand=scroll.set)

    def start_live_monitoring(self):
        self.refresh_file_tree()
        self.root.after(2000, self.start_live_monitoring)

    def update_line_numbers(self, tab_frame):
        if not hasattr(tab_frame, 'editor') or not hasattr(tab_frame, 'line_numbers'):
            return
        if not tab_frame.winfo_exists():
            return

        editor = tab_frame.editor
        line_numbers = tab_frame.line_numbers

        if hasattr(tab_frame, '_line_update_job') and tab_frame._line_update_job:
            self.root.after_cancel(tab_frame._line_update_job)
            tab_frame._line_update_job = None

        def _perform_update():
            if not tab_frame.winfo_exists(): return

            editor.update_idletasks()
            scroll_pos = editor.yview()
            
            line_numbers.config(state='normal')
            line_numbers.delete('1.0', 'end')
            
            content = editor.get('1.0', 'end-1c')
            if not content:
                line_numbers.insert('1.0', '1')
                line_numbers.config(state='disabled')
                return

            full_line_text = []
            current_line = 1
            while True:
                index = f"{current_line}.0"
                next_index = f"{current_line + 1}.0"
                
                if editor.compare(index, '>=', 'end-1c'):
                    break

                dline_info = editor.count(index, next_index, "displaylines")
                num_display_lines = dline_info[0] if dline_info else 1
                
                full_line_text.append(str(current_line))
                for _ in range(num_display_lines - 1):
                    full_line_text.append("")
                
                current_line += 1

            line_numbers.insert('1.0', '\n'.join(full_line_text))
            line_numbers.yview_moveto(scroll_pos[0])
            line_numbers.config(state='disabled')

        tab_frame._line_update_job = self.root.after(10, _perform_update)

    def refresh_file_tree(self):
        self.file_tree.delete(*self.file_tree.get_children())
        self.populate_tree('', self.current_directory)
        self.restore_expansion('', self.current_directory)

    def restore_expansion(self, parent, path):
        for child in self.file_tree.get_children(parent):
            item_values = self.file_tree.item(child, 'values')
            if not item_values: continue
            child_path = item_values[0]
            if child_path in self.expanded_paths:
                self.file_tree.item(child, open=True)
                if self.file_tree.get_children(child):
                      first_child = self.file_tree.get_children(child)[0]
                      if self.file_tree.item(first_child, 'text') == "": 
                          self.file_tree.delete(first_child)
                          self.populate_tree(child, child_path)
                self.restore_expansion(child, child_path)

    def populate_tree(self, parent, path):
        try:
            for item in sorted(os.listdir(path)):
                item_path = os.path.join(path, item)
                if os.path.isdir(item_path):
                    node = self.file_tree.insert(parent, 'end', text=f"📁 {item}", values=[item_path], open=False)
                    self.file_tree.insert(node, 'end') 
                else:
                    self.file_tree.insert(parent, 'end', text=f"📄 {item}", values=[item_path])
        except PermissionError: pass

    def on_tree_open(self, event):
        sel = self.file_tree.selection()
        if not sel: return
        item = sel[0]
        path = self.file_tree.item(item, 'values')[0]
        self.expanded_paths.add(path)
        
    def on_tree_close(self, event):
        sel = self.file_tree.selection()
        if not sel: return
        item = sel[0]
        path = self.file_tree.item(item, 'values')[0]
        if path in self.expanded_paths:
            self.expanded_paths.remove(path)

    def show_context_menu(self, event):
        item = self.file_tree.identify_row(event.y)
        menu = tk.Menu(self.root, tearoff=0, bg=COLORS['bg_sec'], fg=COLORS['fg'], activebackground=COLORS['select_bg'], borderwidth=0)
        
        if item:
            self.file_tree.selection_set(item)
            item_text = self.file_tree.item(item, 'text')
            item_path = self.file_tree.item(item, 'values')[0]
            if "📁" in item_text:
                menu.add_command(label="New File", command=lambda: self.create_item(item_path, "file"))
                menu.add_command(label="New Folder", command=lambda: self.create_item(item_path, "folder"))
                menu.add_separator()
            else:
                menu.add_command(label="Open", command=lambda: self.open_file_in_editor(item_path))
            menu.add_command(label="Rename (F2)", command=lambda: self.rename_item(item))
            menu.add_command(label="Delete", command=lambda: self.delete_item(item))
        else:
            menu.add_command(label="New File Here", command=lambda: self.create_item(self.current_directory, "file"))
            menu.add_command(label="New Folder Here", command=lambda: self.create_item(self.current_directory, "folder"))
        menu.post(event.x_root, event.y_root)

    def show_tab_context_menu(self, event):
        try:
            index = self.notebook.index(f"@{event.x},{event.y}")
            self.notebook.select(index)
            menu = tk.Menu(self.root, tearoff=0, bg=COLORS['bg_sec'], fg=COLORS['fg'], activebackground=COLORS['select_bg'], borderwidth=0)
            menu.add_command(label="Rename File (F2)", command=self.rename_current_tab)
            menu.add_command(label="Close Tab (Ctrl+W)", command=lambda: self.close_tab(index))
            menu.add_separator()
            menu.add_command(label="Close All", command=self.close_all_tabs)
            menu.post(event.x_root, event.y_root)
        except tk.TclError: pass

    def on_tab_middle_click(self, event):
        try:
            index = self.notebook.index(f"@{event.x},{event.y}")
            self.close_tab(index)
        except tk.TclError:
            pass

    def rename_current_tab(self):
        tab = self.get_current_tab()
        if not tab or not tab.filepath:
            messagebox.showwarning("Warning", "Save the file first before renaming.")
            return
        old_path = tab.filepath
        old_name = tab.filename
        parent_dir = os.path.dirname(old_path)
        new_name = simpledialog.askstring("Rename File", f"Rename {old_name} to:", initialvalue=old_name)
        if new_name and new_name != old_name:
            new_path = os.path.join(parent_dir, new_name)
            try:
                os.rename(old_path, new_path)
                tab.filepath = new_path
                tab.filename = new_name
                self.notebook.tab(self.notebook.select(), text=f"{new_name} ")
                for tab_id in self.notebook.tabs():
                    t = self.notebook.nametowidget(tab_id)
                    if t.filepath == old_path:
                        t.filepath = new_path
                        t.filename = new_name
                        self.notebook.tab(tab_id, text=f"{new_name} ")
                self.output_text.insert('end', f"✓ Renamed: {old_name} -> {new_name}\n", 'success')
                self.refresh_file_tree()
            except Exception as e: messagebox.showerror("Error", str(e))

    def rename_item(self, item_id):
        old_path = self.file_tree.item(item_id, 'values')[0]
        old_name = os.path.basename(old_path)
        parent_dir = os.path.dirname(old_path)
        new_name = simpledialog.askstring("Rename", f"Rename {old_name} to:", initialvalue=old_name)
        if new_name and new_name != old_name:
            new_path = os.path.join(parent_dir, new_name)
            try:
                os.rename(old_path, new_path)
                self.refresh_file_tree()
            except Exception as e: messagebox.showerror("Error", str(e))

    def create_item(self, parent_path, item_type):
        name = simpledialog.askstring(f"New {item_type.capitalize()}", f"Enter {item_type} name:")
        if name:
            path = os.path.join(parent_path, name)
            if os.path.exists(path): return
            try:
                if item_type == "file":
                    with open(path, 'w') as f: pass
                    self.refresh_file_tree()
                    self.open_file_in_editor(path)
                else:
                    os.makedirs(path)
                    self.refresh_file_tree()
            except Exception as e: messagebox.showerror("Error", str(e))

    def delete_item(self, item_id):
        path = self.file_tree.item(item_id, 'values')[0]
        if messagebox.askyesno("Confirm Delete", f"Delete {path}?"):
            try:
                if os.path.isdir(path): shutil.rmtree(path)
                else: os.remove(path)
                for tab_id in self.notebook.tabs():
                    tab = self.notebook.nametowidget(tab_id)
                    if tab.filepath == path: self.notebook.forget(tab_id)
                self.refresh_file_tree()
            except Exception as e: messagebox.showerror("Error", str(e))

    def on_file_double_click(self, event):
        sel = self.file_tree.selection()
        if not sel: return
        item = sel[0]
        item_text = self.file_tree.item(item, 'text')
        if "📁" in item_text:
            if self.file_tree.item(item, 'open'):
                self.file_tree.item(item, open=False)
                self.on_tree_close(None)
            else:
                self.file_tree.item(item, open=True)
                if self.file_tree.get_children(item):
                      first_child = self.file_tree.get_children(item)[0]
                      if self.file_tree.item(first_child, 'text') == "":
                          self.file_tree.delete(first_child)
                          self.populate_tree(item, self.file_tree.item(item, 'values')[0])
                self.on_tree_open(None)
        elif "📄" in item_text:
            self.open_file_in_editor(self.file_tree.item(item, 'values')[0])

    def open_file_in_editor(self, filepath):
        for i in range(self.notebook.index('end')):
            tab = self.notebook.nametowidget(self.notebook.tabs()[i])
            if tab.filepath == filepath: self.notebook.select(i); return
        filename = os.path.basename(filepath)
        tab_frame = self.create_new_editor_tab(filename)
        tab_frame.filepath = filepath
        try:
            with open(filepath, 'r') as f:
                content = f.read()
                tab_frame.editor.delete('1.0', 'end')
                tab_frame.editor.insert('1.0', content)
                self.root.after(50, lambda: self.update_line_numbers(tab_frame))
        except Exception as e: messagebox.showerror("Error", str(e))

    def on_tab_click(self, event):
        try:
            index = self.notebook.index(f"@{event.x},{event.y}")
            element = self.notebook.identify(event.x, event.y)
            if "close" in element: self.close_tab(index)
            else: self.notebook.select(index)
        except tk.TclError: pass

    def close_tab(self, index=None):
        if index is None:
            try: index = self.notebook.index('current')
            except tk.TclError: return
        self.notebook.forget(index)

    def close_all_tabs(self):
        while self.notebook.tabs(): self.notebook.forget(0)

    def cycle_tabs(self, direction):
        tabs = self.notebook.tabs()
        if not tabs: return
        current_index = self.notebook.index(self.notebook.select())
        new_index = (current_index + direction) % len(tabs)
        self.notebook.select(tabs[new_index])
        return "break"

    def get_current_tab(self):
        current = self.notebook.select()
        if current: return self.notebook.nametowidget(current)
        return None

    def new_file(self): self.create_new_editor_tab("Untitled")
    
    def open_file(self):
        filepath = filedialog.askopenfilename()
        if filepath: self.open_file_in_editor(filepath)
    
    def open_folder(self):
        folder_path = filedialog.askdirectory(title="Select Project Folder")
        if folder_path:
            self.current_directory = folder_path
            self.expanded_paths.clear()
            self.refresh_file_tree()
            self.output_text.insert('end', f"📁 Opened folder: {folder_path}\n", 'info')
    
    def save_file(self):
        tab = self.get_current_tab()
        if not tab: return
        if tab.filepath:
            try:
                content = tab.editor.get('1.0', 'end-1c')
                with open(tab.filepath, 'w') as f: f.write(content)
                self.output_text.insert('end', f"✓ Saved: {tab.filepath}\n", 'success')
            except Exception as e: messagebox.showerror("Error", str(e))
        else: self.save_file_as()
        
    def save_file_as(self):
        tab = self.get_current_tab()
        if not tab: return
        filepath = filedialog.asksaveasfilename(defaultextension=".a")
        if filepath:
            try:
                content = tab.editor.get('1.0', 'end-1c')
                with open(filepath, 'w') as f: f.write(content)
                tab.filepath = filepath
                tab.filename = os.path.basename(filepath)
                self.notebook.tab(self.notebook.select(), text=f"{tab.filename} ")
                self.output_text.insert('end', f"✓ Saved as: {filepath}\n", 'success')
                self.refresh_file_tree()
            except Exception as e: messagebox.showerror("Error", str(e))

    def clear_output(self): self.output_text.delete('1.0', 'end')

    def compile_and_run(self):
        tab = self.get_current_tab()
        if not tab: return
        if not tab.filepath: self.save_file_as()
        else: self.save_file()
        if not tab.filepath: return
        self.clear_output()
        
        # Modify this path to your compiler
        # NOTE: If you move your final .exe, ensure this path still finds 'a.exe'
        compiler_path = os.path.join('a.exe')
        source_file = tab.filepath
        
        def run_thread():
            if not os.path.exists(compiler_path):
                self.root.after(0, lambda: self.output_text.insert('end', f"❌ Compiler missing: {compiler_path}\n", 'error'))
                return
                
            try:
                # --- START FIX: Hide the Console Window ---
                startupinfo = subprocess.STARTUPINFO()
                startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
                
                # Capture output
                result = subprocess.run(
                    [compiler_path, source_file], 
                    capture_output=True, 
                    text=True, 
                    timeout=30,
                    startupinfo=startupinfo,                   # <--- Add this
                    creationflags=subprocess.CREATE_NO_WINDOW  # <--- Add this
                )
                # --- END FIX ---
                
                # Update GUI safely from thread
                self.root.after(0, lambda: self.output_text.insert('end', result.stdout + "\n"))
                if result.stderr: 
                    self.root.after(0, lambda: self.output_text.insert('end', result.stderr + "\n", 'error'))
            except Exception as e: 
                self.root.after(0, lambda: self.output_text.insert('end', f"❌ Error: {str(e)}\n", 'error'))
        
        thread = threading.Thread(target=run_thread)
        thread.daemon = True
        thread.start()