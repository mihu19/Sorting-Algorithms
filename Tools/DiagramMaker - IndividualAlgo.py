import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

ALGORITHM_COLORS = { 
    'Bubble Sort': ('salmon', 'skyblue'),
    'Insertion Sort': ('mediumpurple', 'lightgreen'),
    'Shell Sort': ('coral', 'teal'),
    'Merge Sort': ('gold', 'dodgerblue'),
    'Hybrid Sort': ('darkorange', 'cadetblue')
}

def get_colors(algorithm):
    return ALGORITHM_COLORS.get(algorithm, ('#ff9999', '#66b3ff'))

def generate_inclined_diagram(csv_file, start_row, end_row):
    if not os.path.exists(csv_file):
        print(f"Error: The file '{csv_file}' was not found.")
        return

    try:
        df = pd.read_csv(csv_file)
    except Exception as e:
        print(f"Error reading CSV: {e}")
        return

    if start_row < 0 or start_row > end_row or start_row >= len(df):
        print(f"Invalid row range, enter numbers between 0 and {len(df)-1}.")
        return
    if end_row >= len(df):
        end_row = len(df) - 1

    df_selected = df.iloc[start_row:end_row + 1].copy()

    df_selected['X_Label'] = df_selected['Size'].astype(str) + " (" + df_selected['Array Type'] + ")"
    mode = df_selected['Mode'].iloc[0]
    algo = df_selected['Algorithm'].iloc[0]
    time_color, mem_color = get_colors(algo)

    num_bars = len(df_selected)
    dynamic_width = max(16, num_bars * 1.0) 
    
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(dynamic_width, 12), sharex=True)
    fig.suptitle(f"Benchmark Performance: {algo} ({mode})", fontsize=20, fontweight='bold')

    #hardcoded log scale for accuracy across highly variable timeframes
    time_scale = 'log' 

    #execution time
    bars1 = ax1.bar(df_selected['X_Label'], df_selected['Time (Seconds)'], color=time_color, edgecolor='black')
    
    ax1.set_yscale(time_scale) 
    ax1.set_ylabel(f"Time (Seconds) - {time_scale.upper()} SCALE", fontsize=12, fontweight='bold')
    ax1.set_title(f"Execution Time ({time_scale.capitalize()} Scale)", fontsize=14)
    ax1.grid(axis='y', linestyle='--', alpha=0.7)
    
    ax1.margins(x=0.06, y=0.15) 
    
    for bar in bars1:
        yval = bar.get_height()
        y_pos = yval * 1.25
        label_text = f"{yval:.2e}" if yval < 0.001 else f"{yval:.4f}"
        
        ax1.text(bar.get_x() + bar.get_width()/2, y_pos, label_text, 
                 ha='center', va='bottom', fontsize=10, rotation=45, color='black', fontweight='bold')

    #memory
    bars2 = ax2.bar(df_selected['X_Label'], df_selected['Memory (MB)'], color=mem_color, edgecolor='black')
    
    min_mem = df_selected['Memory (MB)'].min()
    max_mem = df_selected['Memory (MB)'].max()
    padding = (max_mem - min_mem) * 0.2 if max_mem != min_mem else 0.5
    
    ax2.set_ylim(max(0, min_mem - padding), max_mem + padding * 3)
    ax2.set_ylabel("Memory (MB)", fontsize=12, fontweight='bold')
    ax2.set_title("Memory Consumption", fontsize=14)
    ax2.grid(axis='y', linestyle='--', alpha=0.7)
    
    ax2.margins(x=0.06, y=0.15)
    
    for bar in bars2:
        yval = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width()/2, yval + (padding*0.1), f"{yval:.4f}", 
                 ha='center', va='bottom', fontsize=10, rotation=45, color='black', fontweight='bold')

    ax2.set_xticks(range(len(df_selected)))
    ax2.set_xticklabels(df_selected['X_Label'], rotation=45, ha='right', fontsize=11, fontweight='bold')
    
    fig.tight_layout(rect=[0, 0.05, 1, 0.94], h_pad=2.0)
    
    #saving file
    os.makedirs('images', exist_ok=True)
    safe_algo_name = algo.replace(" ", "_").lower()
    filename = os.path.join('images', f'benchmark_{safe_algo_name}_log.png')
    plt.savefig(filename, bbox_inches='tight') 
    print(f"\nSaved chart as '{filename}'.")
    plt.show()

if __name__ == "__main__":
    file_path = "benchmark_results.csv" 
    
    print("Algorithms Benchmark -> Diagram")
    
    try:
        start_input = input("Enter the START row (i - 2): ").strip()
        end_input = input("Enter the END row (i - 2): ").strip()
            
        start_idx = int(start_input)
        end_idx = int(end_input)
        
        generate_inclined_diagram(file_path, start_idx, end_idx)
        
    except ValueError:
        print("\n=enter valid row numbers only.")
        sys.exit(1)
    except KeyboardInterrupt:
        print("\ncanceled")

        sys.exit(0)
