import pandas as pd
import matplotlib.pyplot as plt

ALGORITHM_COLORS = { 
    'Bubble Sort': ('salmon', 'skyblue'),
    'Insertion Sort': ('mediumpurple', 'lightgreen'),
    'Shell Sort': ('coral', 'teal'),
    'Merge Sort': ('gold', 'dodgerblue'),
    'Hybrid Sort': ('darkorange', 'cadetblue')
}

def generate_diagram():
    filename = 'benchmark_results.csv'
    try:
        df = pd.read_csv(filename)
    except FileNotFoundError:
        print(f"Error: Could not find '{filename}'")
        return

    #row inputs
    try:
        start_input = input("Enter the START row (i - 2): ").strip()
        end_input = input("Enter the END row (i - 2): ").strip()
        start_idx = int(start_input)
        end_idx = int(end_input)
    except ValueError:
        print("Invalid input, enter valid integers.")
        return

    subset_df = df.iloc[start_idx : end_idx + 1].copy()
    if subset_df.empty:
        print("selected range is empty")
        return

    # force numeric values to avoid graphing errors
    subset_df['Size'] = pd.to_numeric(subset_df['Size'], errors='coerce')
    subset_df['Time (Seconds)'] = pd.to_numeric(subset_df['Time (Seconds)'], errors='coerce')
    subset_df['Memory (MB)'] = pd.to_numeric(subset_df['Memory (MB)'], errors='coerce')

    metric = input("Choose the metric for the Y-axis ('time' or 'memory'): ").strip().lower()
    
    if metric == 'memory':
        y_col = 'Memory (MB)'
        color_idx = 1
        y_label = 'Memory Used (MB)'
        title = 'Algorithm Memory Usage by Array Size(Mixed (+/-))'
    else:
        y_col = 'Time (Seconds)'
        color_idx = 0
        y_label = 'Time (Seconds) - Log Scale'
        title = 'Algorithm Sorting Time by Array Size(Mixed (+/-))'

    pivot_df = subset_df.pivot_table(index='Size', columns='Algorithm', values=y_col, aggfunc='mean')

    bar_colors = [ALGORITHM_COLORS.get(algo, ('grey', 'grey'))[color_idx] for algo in pivot_df.columns]

    # generate the Grouped Bar Chart
    ax = pivot_df.plot(
        kind='bar', 
        figsize=(12, 7), 
        color=bar_colors, 
        edgecolor='black', #clean border to each bar for maximum visibility
        linewidth=1
    )

    # apply scaling
    if metric == 'time':
        ax.set_yscale('log')
    else:
        #zoom for memory so the bars dont all look the exact same height
        min_val = pivot_df.min().min()
        max_val = pivot_df.max().max()
        buffer = (max_val - min_val) * 0.1 if max_val != min_val else 0.5
        ax.set_ylim(max(0, min_val - buffer), max_val + buffer)

    plt.title(title, fontsize=14, fontweight='bold')
    plt.xlabel('Array Size', fontsize=12)
    plt.ylabel(y_label, fontsize=12)
    plt.xticks(rotation=0) 
    plt.grid(True, axis='y', linestyle='--', alpha=0.6) 
    
    # move legend  outside the chart area
    plt.legend(title="Algorithms", bbox_to_anchor=(1.05, 1), loc='upper left')
    
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":

    generate_diagram()
