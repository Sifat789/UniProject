import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.*;
import java.util.List;
import java.util.Queue;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

// Main class extending JFrame for the GUI
public class OsSimulatorGUI extends JFrame {

    // --- GUI Components ---
    private JTabbedPane tabbedPane;
    private JPanel processPanel, diskPanel, pagePanel;

    // Process Scheduling Components
    private JTextArea processInputArea;
    private JTextField quantumText;
    private JComboBox<String> processAlgoDropdown;
    private JTextArea processOutputArea;
    private JButton processRunButton;
    private final String PROCESS_PLACEHOLDER = "P1,0,5,2\nP2,1,3,1\nP3,2,8,3"; // Example format

    // Disk Scheduling Components
    private JTextArea diskQueueArea;
    private JTextField headStartText;
    private JTextField diskSizeText; // Max cylinder number (e.g., 199 for 0-199)
    private JComboBox<String> diskAlgoDropdown;
    private JTextArea diskOutputArea;
    private JButton diskRunButton;
    private final String DISK_QUEUE_PLACEHOLDER = "98\n183\n37\n122\n14\n124\n65\n67"; // Example
    private final String DISK_SIZE_PLACEHOLDER = "199"; // Example (0-199)

    // Page Replacement Components
    private JTextArea pageStringArea;
    private JTextField frameCapacityText;
    private JComboBox<String> pageAlgoDropdown;
    private JTextArea pageOutputArea;
    private JButton pageRunButton;
    private final String PAGE_STRING_PLACEHOLDER = "7\n0\n1\n2\n0\n3\n0\n4\n2\n3\n0\n3"; // Example

    // --- Constructor ---
    public OsSimulatorGUI() {
        // Frame setup
        setTitle("OS Algorithm Simulator (Simple GUI)");
        setSize(850, 650); // Increased size slightly
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Create Tabbed Pane
        tabbedPane = new JTabbedPane();

        // Create Panels for each category
        createProcessPanel();
        createDiskPanel();
        createPagePanel();

        // Add panels to tabbed pane
        tabbedPane.addTab("Process Scheduling", processPanel);
        tabbedPane.addTab("Disk Scheduling", diskPanel);
        tabbedPane.addTab("Page Replacement", pagePanel);

        // Add tabbed pane to frame
        add(tabbedPane, BorderLayout.CENTER);

        // Make frame visible
        setLocationRelativeTo(null); // Center on screen
        setVisible(true);
    }

    // --- GUI Panel Creation Methods ---

    private void createProcessPanel() {
        processPanel = new JPanel(new BorderLayout(10, 10));
        processPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // Padding

        // --- Input Panel (Top) ---
        JPanel inputConfigPanel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(3, 5, 3, 5); // Padding
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.WEST; // Align labels left

        // Algorithm Selection
        gbc.gridx = 0; gbc.gridy = 0; gbc.gridwidth = 1; gbc.weightx = 0.0; // Label weight 0
        inputConfigPanel.add(new JLabel("Algorithm:"), gbc);

        gbc.gridx = 1; gbc.gridy = 0; gbc.gridwidth = 1; gbc.weightx = 1.0; // Dropdown takes space
        processAlgoDropdown = new JComboBox<>(new String[]{
                "First Come First Serve (FCFS)",
                "Shortest Job First (SJF) - Non-preemptive",
                "Priority Scheduling - Non-preemptive",
                "Shortest Job First (SJF) - Preemptive",
                "Round Robin (RR)",
                "Priority Scheduling - Preemptive"
        });
        inputConfigPanel.add(processAlgoDropdown, gbc);

        // Quantum Input (only relevant for RR)
        gbc.gridx = 0; gbc.gridy = 1; gbc.gridwidth = 1; gbc.weightx = 0.0;
        inputConfigPanel.add(new JLabel("Quantum (for RR):"), gbc);

        gbc.gridx = 1; gbc.gridy = 1; gbc.gridwidth = 1; gbc.weightx = 1.0;
        quantumText = new JTextField("2", 5);
        inputConfigPanel.add(quantumText, gbc);

        // --- Process Input Area (Center) ---
        JPanel processInputPanel = new JPanel(new BorderLayout(0, 5)); // Panel for label + text area
        processInputPanel.add(new JLabel("Processes (One per line: ID,Arrival,Burst[,Priority]):"), BorderLayout.NORTH);

        processInputArea = new JTextArea(PROCESS_PLACEHOLDER, 8, 40); // Rows, Columns
        processInputArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        JScrollPane processInputScrollPane = new JScrollPane(processInputArea);
        processInputPanel.add(processInputScrollPane, BorderLayout.CENTER);

        // --- Run Button (Bottom Right) ---
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT)); // Align button right
        processRunButton = new JButton("Run Process Scheduling");
        buttonPanel.add(processRunButton);

        // --- Output Area ---
        processOutputArea = new JTextArea();
        processOutputArea.setEditable(false);
        processOutputArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        JScrollPane processOutputScrollPane = new JScrollPane(processOutputArea);

        // --- Assembly ---
        JPanel topPanel = new JPanel(new BorderLayout(10, 10)); // Combine config and input area
        topPanel.add(inputConfigPanel, BorderLayout.NORTH);
        topPanel.add(processInputPanel, BorderLayout.CENTER);
        topPanel.add(buttonPanel, BorderLayout.SOUTH);

        processPanel.add(topPanel, BorderLayout.NORTH); // All inputs and button at the top
        processPanel.add(processOutputScrollPane, BorderLayout.CENTER); // Output below

        // --- Action Listeners ---
        processRunButton.addActionListener(e -> runProcessScheduling());
        processAlgoDropdown.addActionListener(e -> { // Enable/disable quantum field
            String selected = (String) processAlgoDropdown.getSelectedItem();
            quantumText.setEnabled(selected != null && selected.equals("Round Robin (RR)"));
        });
        quantumText.setEnabled(false); // Initially disabled
    }
    
    private void createDiskPanel() {
        diskPanel = new JPanel(new BorderLayout(10, 10));
        diskPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // --- Input Panel (Top) ---
        JPanel inputConfigPanel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(3, 5, 3, 5);
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.WEST;

        // Algorithm
        gbc.gridx = 0; gbc.gridy = 0; gbc.weightx = 0.0;
        inputConfigPanel.add(new JLabel("Algorithm:"), gbc);
        gbc.gridx = 1; gbc.weightx = 1.0;
        diskAlgoDropdown = new JComboBox<>(new String[]{
                "First Come First Serve (FCFS)",
                "Shortest Seek Time First (SSTF)",
                "SCAN (Elevator)",
                "C-SCAN (Circular SCAN)",
                "LOOK",
                "C-LOOK (Circular LOOK)"
        });
        inputConfigPanel.add(diskAlgoDropdown, gbc);

        // Head Start
        gbc.gridx = 0; gbc.gridy = 1; gbc.weightx = 0.0;
        inputConfigPanel.add(new JLabel("Head Start Position:"), gbc);
        gbc.gridx = 1; gbc.weightx = 1.0;
        headStartText = new JTextField("53", 5);
        inputConfigPanel.add(headStartText, gbc);

        // Disk Size
        gbc.gridx = 0; gbc.gridy = 2; gbc.weightx = 0.0;
        inputConfigPanel.add(new JLabel("Disk Size (Max Cylinder#):"), gbc);
        gbc.gridx = 1; gbc.weightx = 1.0;
        diskSizeText = new JTextField(DISK_SIZE_PLACEHOLDER, 5);
        inputConfigPanel.add(diskSizeText, gbc);


        // --- Disk Queue Input Area (Center) ---
        JPanel diskInputPanel = new JPanel(new BorderLayout(0, 5));
        diskInputPanel.add(new JLabel("Request Queue (One request per line):"), BorderLayout.NORTH);

        diskQueueArea = new JTextArea(DISK_QUEUE_PLACEHOLDER, 8, 15); // Rows, Columns
        diskQueueArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        JScrollPane diskInputScrollPane = new JScrollPane(diskQueueArea);
        diskInputPanel.add(diskInputScrollPane, BorderLayout.CENTER);

        // --- Run Button (Bottom Right) ---
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        diskRunButton = new JButton("Run Disk Scheduling");
        buttonPanel.add(diskRunButton);

        // --- Output Area ---
        diskOutputArea = new JTextArea();
        diskOutputArea.setEditable(false);
        diskOutputArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        JScrollPane diskOutputScrollPane = new JScrollPane(diskOutputArea);

        // --- Assembly ---
        JPanel topPanel = new JPanel(new BorderLayout(10, 10));
        topPanel.add(inputConfigPanel, BorderLayout.NORTH);
        topPanel.add(diskInputPanel, BorderLayout.CENTER);
        topPanel.add(buttonPanel, BorderLayout.SOUTH);

        diskPanel.add(topPanel, BorderLayout.NORTH);
        diskPanel.add(diskOutputScrollPane, BorderLayout.CENTER);

        // --- Action Listener ---
        diskRunButton.addActionListener(e -> runDiskScheduling());
    }

    private void createPagePanel() {
        pagePanel = new JPanel(new BorderLayout(10, 10));
        pagePanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // --- Input Panel (Top) ---
        JPanel inputConfigPanel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(3, 5, 3, 5);
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.WEST;

        // Algorithm
        gbc.gridx = 0; gbc.gridy = 0; gbc.weightx = 0.0;
        inputConfigPanel.add(new JLabel("Algorithm:"), gbc);
        gbc.gridx = 1; gbc.weightx = 1.0;
        pageAlgoDropdown = new JComboBox<>(new String[]{
                "First-In First-Out (FIFO)",
                "Least Recently Used (LRU)",
                "Optimal Page Replacement"
        });
        inputConfigPanel.add(pageAlgoDropdown, gbc);

        // Frame Capacity
        gbc.gridx = 0; gbc.gridy = 1; gbc.weightx = 0.0;
        inputConfigPanel.add(new JLabel("Frame Capacity:"), gbc);
        gbc.gridx = 1; gbc.weightx = 1.0;
        frameCapacityText = new JTextField("3", 5);
        inputConfigPanel.add(frameCapacityText, gbc);


        // --- Page String Input Area (Center) ---
        JPanel pageInputPanel = new JPanel(new BorderLayout(0, 5));
        pageInputPanel.add(new JLabel("Reference String (One page number per line):"), BorderLayout.NORTH);

        pageStringArea = new JTextArea(PAGE_STRING_PLACEHOLDER, 8, 15); // Rows, Columns
        pageStringArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        JScrollPane pageInputScrollPane = new JScrollPane(pageStringArea);
        pageInputPanel.add(pageInputScrollPane, BorderLayout.CENTER);


        // --- Run Button (Bottom Right) ---
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        pageRunButton = new JButton("Run Page Replacement");
        buttonPanel.add(pageRunButton);

        // --- Output Area ---
        pageOutputArea = new JTextArea();
        pageOutputArea.setEditable(false);
        pageOutputArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        JScrollPane pageOutputScrollPane = new JScrollPane(pageOutputArea);

        // --- Assembly ---
        JPanel topPanel = new JPanel(new BorderLayout(10, 10));
        topPanel.add(inputConfigPanel, BorderLayout.NORTH);
        topPanel.add(pageInputPanel, BorderLayout.CENTER);
        topPanel.add(buttonPanel, BorderLayout.SOUTH);

        pagePanel.add(topPanel, BorderLayout.NORTH);
        pagePanel.add(pageOutputScrollPane, BorderLayout.CENTER);

        // --- Action Listener ---
        pageRunButton.addActionListener(e -> runPageReplacement());
    }

    // --- Event Handlers / Trigger Methods ---

    private void runProcessScheduling() {
        try {
            String algorithm = (String) processAlgoDropdown.getSelectedItem();
            String inputText = processInputArea.getText();
            int quantum = 0;
            if (algorithm != null && algorithm.equals("Round Robin (RR)")) {
                quantum = Integer.parseInt(quantumText.getText());
                if (quantum <= 0) throw new NumberFormatException("Quantum must be positive.");
            }

            List<ProcessData> processes = parseProcessInputFromArea(inputText);
            if (processes == null || processes.isEmpty()) {
                processOutputArea.setText("Error parsing process input or no processes found.\nFormat: ID,Arrival,Burst[,Priority]; ID,Arrival,Burst[,Priority]...\nExample: P1,0,5,2; P2,1,3,1");
                return;
            }
             // Make a mutable copy for algorithms to modify
            List<ProcessData> mutableProcesses = processes.stream()
                                                        .map(ProcessData::new) // Use copy constructor
                                                        .collect(Collectors.toList());

            String result = "";
            switch (algorithm) {
                case "First Come First Serve (FCFS)":
                    result = runFCFS(mutableProcesses);
                    break;
                case "Shortest Job First (SJF) - Non-preemptive":
                    result = runSJF_NP(mutableProcesses);
                    break;
                case "Priority Scheduling - Non-preemptive":
                    result = runPriority_NP(mutableProcesses);
                    break;
                case "Shortest Job First (SJF) - Preemptive":
                    result = runSJF_P(mutableProcesses);
                    break;
                case "Round Robin (RR)":
                    result = runRR(mutableProcesses, quantum);
                    break;
                case "Priority Scheduling - Preemptive":
                    result = runPriority_P(mutableProcesses);
                    break;
                default:
                    result = "Algorithm not selected or not implemented yet.";
            }
            processOutputArea.setText(result);

        } catch (NumberFormatException ex) {
            processOutputArea.setText("Invalid number format in input.\nPlease check Arrival, Burst, Priority (if used), and Quantum (if RR): " + ex.getMessage());
        } catch (Exception ex) {
            processOutputArea.setText("An error occurred: " + ex.getMessage());
            // ex.printStackTrace(); // Uncomment for detailed debugging in console
        }
    }

    private void runDiskScheduling() {
        try {
            String algorithm = (String) diskAlgoDropdown.getSelectedItem();
            List<Integer> queue = parseIntegerListFromArea(diskQueueArea.getText());
            int headStart = Integer.parseInt(headStartText.getText());
            int diskSize = Integer.parseInt(diskSizeText.getText()); // Max cylinder number

            if (queue == null || queue.isEmpty()) {
                diskOutputArea.setText("Error parsing disk queue or queue is empty.\nFormat: req1,req2,... Example: 98,183,37");
                return;
            }
             if (headStart < 0 || headStart > diskSize) {
                diskOutputArea.setText("Head start position must be between 0 and " + diskSize);
                return;
            }
             for (int req : queue) {
                 if (req < 0 || req > diskSize) {
                     diskOutputArea.setText("Request " + req + " is outside disk bounds (0-" + diskSize + ")");
                     return;
                 }
             }


            String result = "";
            // Pass copies of the queue as some algorithms modify it
            switch (algorithm) {
                case "First Come First Serve (FCFS)":
                    result = runDiskFCFS(new ArrayList<>(queue), headStart);
                    break;
                case "Shortest Seek Time First (SSTF)":
                    result = runSSTF(new ArrayList<>(queue), headStart);
                    break;
                case "SCAN (Elevator)":
                    // Simple assumption: start moving towards higher numbers if head < diskSize/2
                    boolean startMovingRightScan = headStart <= diskSize / 2;
                    result = runSCAN(new ArrayList<>(queue), headStart, diskSize, startMovingRightScan);
                    break;
                case "C-SCAN (Circular SCAN)":
                    boolean startMovingRightCScan = headStart <= diskSize / 2;
                    result = runCSCAN(new ArrayList<>(queue), headStart, diskSize, startMovingRightCScan);
                    break;
                case "LOOK":
                     boolean startMovingRightLook = headStart <= diskSize / 2;
                    result = runLOOK(new ArrayList<>(queue), headStart, startMovingRightLook);
                    break;
                case "C-LOOK (Circular LOOK)":
                     boolean startMovingRightCLook = headStart <= diskSize / 2;
                    result = runCLOOK(new ArrayList<>(queue), headStart, startMovingRightCLook);
                    break;
                default:
                    result = "Algorithm not selected or not implemented yet.";
            }
            diskOutputArea.setText(result);

        } catch (NumberFormatException ex) {
            diskOutputArea.setText("Invalid number format in input.\nPlease check Head Start, Disk Size, and Queue values: " + ex.getMessage());
        } catch (Exception ex) {
            diskOutputArea.setText("An error occurred: " + ex.getMessage());
             // ex.printStackTrace(); // Uncomment for detailed debugging in console
        }
    }

    private void runPageReplacement() {
        try {
            String algorithm = (String) pageAlgoDropdown.getSelectedItem();
            List<Integer> refString = parseIntegerListFromArea(pageStringArea.getText());
            int capacity = Integer.parseInt(frameCapacityText.getText());

            if (refString == null || refString.isEmpty()) {
                pageOutputArea.setText("Error parsing reference string or string is empty.\nFormat: page1,page2,... Example: 7,0,1");
                return;
            }
            if (capacity <= 0) {
                pageOutputArea.setText("Frame capacity must be positive.");
                return;
            }

            String result = "";
            switch (algorithm) {
                case "First-In First-Out (FIFO)":
                    result = runFIFO(refString, capacity);
                    break;
                case "Least Recently Used (LRU)":
                    result = runLRU(refString, capacity);
                    break;
                case "Optimal Page Replacement":
                    result = runOptimal(refString, capacity);
                    break;
                default:
                    result = "Algorithm not selected or not implemented yet.";
            }
            pageOutputArea.setText(result);

        } catch (NumberFormatException ex) {
            pageOutputArea.setText("Invalid number format in input.\nPlease check Capacity and Reference String values: " + ex.getMessage());
        } catch (Exception ex) {
            pageOutputArea.setText("An error occurred: " + ex.getMessage());
            // ex.printStackTrace(); // Uncomment for detailed debugging in console
        }
    }

    // --- Input Parsing Helpers ---

    // --- Input Parsing Helpers (Modified for JTextArea) ---

    // --- Input Parsing Helpers (Revised to better skip headers/invalid lines) ---

    private List<Integer> parseIntegerListFromArea(String text) {
        if (text == null || text.trim().isEmpty()) return null;
        List<Integer> numbers = new ArrayList<>();
        String[] lines = text.trim().split("\\r?\\n"); // Split by newline
        int lineNumber = 0;
        List<String> parsingErrors = new ArrayList<>(); // Collect errors

        for (String line : lines) {
            lineNumber++;
            String trimmedLine = line.trim();

            // Skip empty lines OR lines that clearly aren't just a number
            if (trimmedLine.isEmpty() || !trimmedLine.matches("-?\\d+")) {
                // If it's not empty and not a number, maybe log it or ignore silently
                // For simplicity, we'll just ignore it assuming it's a comment/header
                continue;
            }

            try {
                numbers.add(Integer.parseInt(trimmedLine));
            } catch (NumberFormatException e) {
                // This might happen if the number is too large/small for int
                parsingErrors.add(String.format("Line %d: Invalid number format '%s'", lineNumber, trimmedLine));
            }
        }

        // If errors occurred, report them and return null
        if (!parsingErrors.isEmpty()) {
            throw new IllegalArgumentException(String.join("\n", parsingErrors));
        }

        return numbers.isEmpty() ? null : numbers; // Return null if only headers/comments were present
    }

    private List<ProcessData> parseProcessInputFromArea(String text) {
        if (text == null || text.trim().isEmpty()) return null;
        List<ProcessData> processes = new ArrayList<>();
        String[] lines = text.trim().split("\\r?\\n"); // Split by newline
        int lineNumber = 0;
        List<String> parsingErrors = new ArrayList<>(); // Collect errors

        for (String line : lines) {
            lineNumber++;
            String trimmedLine = line.trim();

            // Skip empty lines, comments, or lines that don't contain commas (likely headers)
            if (trimmedLine.isEmpty() || trimmedLine.startsWith("#") || trimmedLine.startsWith("//") || !trimmedLine.contains(",")) {
                continue;
            }

            String[] parts = trimmedLine.split(",");
            if (parts.length < 3 || parts.length > 4) {
                parsingErrors.add(String.format("Line %d: Invalid format (%d parts found). Expected ID,Arrival,Burst[,Priority]", lineNumber, parts.length));
                continue; // Skip to next line on format error
            }

            try {
                String id = parts[0].trim();
                if (id.isEmpty()) {
                    parsingErrors.add(String.format("Line %d: Process ID cannot be empty.", lineNumber));
                    continue;
                }

                int arrival = Integer.parseInt(parts[1].trim());
                int burst = Integer.parseInt(parts[2].trim());
                int priority = (parts.length == 4) ? Integer.parseInt(parts[3].trim()) : 0;

                if (arrival < 0 || burst <= 0) {
                   parsingErrors.add(String.format("Line %d (%s): Arrival must be >= 0 and Burst must be > 0.", lineNumber, id));
                   continue;
                }

                processes.add(new ProcessData(id, arrival, burst, priority));

            } catch (NumberFormatException e) {
                 parsingErrors.add(String.format("Line %d: Invalid number format in '%s' - %s", lineNumber, trimmedLine, e.getMessage()));
                 // Continue processing other lines even if one has a number format error
            } catch (IllegalArgumentException e) { // Catch other validation errors if any were added
                 parsingErrors.add(String.format("Line %d: Validation error - %s", lineNumber, e.getMessage()));
                 // Continue processing other lines
            }
        }

        // If errors occurred, report them BUT still return processes if some were valid
         if (!parsingErrors.isEmpty()) {
            // Display errors in the output area (modify the run... methods to handle this better later if needed)
            // For now, just throw the first error encountered, or a summary
            throw new IllegalArgumentException("Errors during parsing:\n" + String.join("\n", parsingErrors));
         }

        // Check if any valid processes were actually parsed after skipping/errors
        if (processes.isEmpty()) {
             throw new IllegalArgumentException("No valid process data found in the input area.");
        }

        return processes;
    }
    
    // --- Static Inner Class for Process Data (using standard class for wider Java compatibility) ---
    static class ProcessData {
        String id;
        int arrivalTime;
        int burstTime;
        int priority;

        int remainingTime;
        int completionTime;
        int turnaroundTime;
        int waitingTime;
        int startTime = -1; // Time execution starts/resumes

        // Constructor for initial data
        public ProcessData(String id, int arrivalTime, int burstTime, int priority) {
            this.id = id;
            this.arrivalTime = arrivalTime;
            this.burstTime = burstTime;
            this.priority = priority;
            this.remainingTime = burstTime;
            // Other times initialized later
            this.completionTime = 0;
            this.turnaroundTime = 0;
            this.waitingTime = 0;
        }

         // Copy constructor
        public ProcessData(ProcessData other) {
            this.id = other.id;
            this.arrivalTime = other.arrivalTime;
            this.burstTime = other.burstTime;
            this.priority = other.priority;
            this.remainingTime = other.remainingTime;
            this.completionTime = other.completionTime;
            this.turnaroundTime = other.turnaroundTime;
            this.waitingTime = other.waitingTime;
            this.startTime = other.startTime;
        }

        // Calculate final metrics
        public void calculateMetrics(int ct) {
            this.completionTime = ct;
            this.turnaroundTime = this.completionTime - this.arrivalTime;
            this.waitingTime = this.turnaroundTime - this.burstTime;
             // Ensure waiting time isn't negative due to potential rounding/timing issues
            if (this.waitingTime < 0) this.waitingTime = 0;
        }

        @Override
        public String toString() {
            return String.format("ID: %s, AT: %d, BT: %d, Prio: %d, RT: %d, CT: %d, TAT: %d, WT: %d",
                                 id, arrivalTime, burstTime, priority, remainingTime, completionTime, turnaroundTime, waitingTime);
        }
    }

     // Helper class for Gantt Chart events
    static class GanttEvent {
        String processId;
        int startTime;
        int endTime;

        GanttEvent(String processId, int startTime, int endTime) {
            this.processId = processId;
            this.startTime = startTime;
            this.endTime = endTime;
        }
         @Override
        public String toString() {
            return String.format("| %s [%d-%d] ", processId, startTime, endTime);
        }
    }

    // --- Algorithm Implementations ---

    // == Process Scheduling Algorithms ==

    // Helper to calculate and format final process results
    private String formatProcessResults(List<ProcessData> finishedProcesses, List<GanttEvent> ganttChart) {
         StringBuilder sb = new StringBuilder();
         sb.append("Gantt Chart:\n");
         int lastEndTime = 0;
         for(GanttEvent event : ganttChart) {
             // Add idle time if needed
             if (event.startTime > lastEndTime) {
                 sb.append(String.format("| IDLE [%d-%d] ", lastEndTime, event.startTime));
             }
             sb.append(event.toString());
             lastEndTime = event.endTime;
         }
         sb.append("|\n\n");


         sb.append(String.format("%-5s %-10s %-10s %-10s %-10s %-10s %-10s\n",
                                 "PID", "Arrival", "Burst", "Priority", "Completion", "Turnaround", "Waiting"));
         sb.append("---------------------------------------------------------------------\n");

         double totalWaitingTime = 0;
         double totalTurnaroundTime = 0;
         // Sort by ID for consistent output order before printing
         finishedProcesses.sort(Comparator.comparing(p -> p.id));

         for (ProcessData p : finishedProcesses) {
             sb.append(String.format("%-5s %-10d %-10d %-10d %-10d %-10d %-10d\n",
                                     p.id, p.arrivalTime, p.burstTime, p.priority,
                                     p.completionTime, p.turnaroundTime, p.waitingTime));
             totalWaitingTime += p.waitingTime;
             totalTurnaroundTime += p.turnaroundTime;
         }
         sb.append("---------------------------------------------------------------------\n");
         sb.append(String.format("Average Waiting Time: %.2f\n", totalWaitingTime / finishedProcesses.size()));
         sb.append(String.format("Average Turnaround Time: %.2f\n", totalTurnaroundTime / finishedProcesses.size()));
         return sb.toString();
    }

    // 1. First Come First Serve (FCFS)
    private String runFCFS(List<ProcessData> processes) {
        processes.sort(Comparator.comparingInt(p -> p.arrivalTime)); // Sort by arrival time

        int currentTime = 0;
        List<ProcessData> finishedProcesses = new ArrayList<>();
        List<GanttEvent> ganttChart = new ArrayList<>();

        for (ProcessData p : processes) {
            if (currentTime < p.arrivalTime) {
                currentTime = p.arrivalTime; // Wait if CPU is idle
            }
            int startTime = currentTime;
            p.startTime = startTime;
            currentTime += p.burstTime; // Execute the process
            p.calculateMetrics(currentTime);
            finishedProcesses.add(p);
             ganttChart.add(new GanttEvent(p.id, startTime, currentTime));
        }
        return formatProcessResults(finishedProcesses, ganttChart);
    }

    // 2. Shortest Job First (SJF) - Non-preemptive
    private String runSJF_NP(List<ProcessData> processes) {
        List<ProcessData> readyQueue = new ArrayList<>();
        List<ProcessData> remainingProcesses = new ArrayList<>(processes);
        List<ProcessData> finishedProcesses = new ArrayList<>();
        List<GanttEvent> ganttChart = new ArrayList<>();
        remainingProcesses.sort(Comparator.comparingInt(p -> p.arrivalTime)); // Sort by arrival

        int currentTime = 0;
        int completedCount = 0;
        int processIndex = 0;

        while (completedCount < processes.size()) {
            // Add processes that have arrived by currentTime to ready queue
            while (processIndex < remainingProcesses.size() && remainingProcesses.get(processIndex).arrivalTime <= currentTime) {
                readyQueue.add(remainingProcesses.get(processIndex));
                processIndex++;
            }

            if (readyQueue.isEmpty()) {
                // If ready queue is empty, advance time to the next arrival
                if (processIndex < remainingProcesses.size()) {
                     currentTime = remainingProcesses.get(processIndex).arrivalTime;
                } else {
                     break; // Should not happen if completedCount < processes.size()
                }
                continue; // Re-check for arrived processes at the new current time
            }

            // Select the process with the shortest burst time from ready queue
            readyQueue.sort(Comparator.comparingInt(p -> p.burstTime));
            ProcessData currentProcess = readyQueue.remove(0);

            int startTime = currentTime;
            currentProcess.startTime = startTime;
            currentTime += currentProcess.burstTime; // Execute fully
            currentProcess.calculateMetrics(currentTime);
            finishedProcesses.add(currentProcess);
            ganttChart.add(new GanttEvent(currentProcess.id, startTime, currentTime));
            completedCount++;
        }
         return formatProcessResults(finishedProcesses, ganttChart);
    }

    // 3. Priority Scheduling - Non-preemptive (Lower number means higher priority)
    private String runPriority_NP(List<ProcessData> processes) {
        List<ProcessData> readyQueue = new ArrayList<>();
        List<ProcessData> remainingProcesses = new ArrayList<>(processes);
        List<ProcessData> finishedProcesses = new ArrayList<>();
         List<GanttEvent> ganttChart = new ArrayList<>();
        remainingProcesses.sort(Comparator.comparingInt(p -> p.arrivalTime));

        int currentTime = 0;
        int completedCount = 0;
        int processIndex = 0;

        while (completedCount < processes.size()) {
            // Add arrived processes
            while (processIndex < remainingProcesses.size() && remainingProcesses.get(processIndex).arrivalTime <= currentTime) {
                readyQueue.add(remainingProcesses.get(processIndex));
                processIndex++;
            }

            if (readyQueue.isEmpty()) {
                 if (processIndex < remainingProcesses.size()) {
                     currentTime = remainingProcesses.get(processIndex).arrivalTime;
                } else {
                     break;
                }
                 continue;
            }

            // Select process with highest priority (lowest number)
            readyQueue.sort(Comparator.comparingInt(p -> p.priority));
            ProcessData currentProcess = readyQueue.remove(0);

            int startTime = currentTime;
            currentProcess.startTime = startTime;
            currentTime += currentProcess.burstTime;
            currentProcess.calculateMetrics(currentTime);
            finishedProcesses.add(currentProcess);
            ganttChart.add(new GanttEvent(currentProcess.id, startTime, currentTime));
            completedCount++;
        }
         return formatProcessResults(finishedProcesses, ganttChart);
    }

     // 4. Shortest Job First (SJF) - Preemptive (Shortest Remaining Time First - SRTF)
    private String runSJF_P(List<ProcessData> processes) {
        PriorityQueue<ProcessData> readyQueue = new PriorityQueue<>(Comparator.comparingInt(p -> p.remainingTime));
        List<ProcessData> remainingProcesses = new ArrayList<>(processes);
        List<ProcessData> finishedProcesses = new ArrayList<>();
        List<GanttEvent> ganttChart = new ArrayList<>();
        remainingProcesses.sort(Comparator.comparingInt(p -> p.arrivalTime));

        int currentTime = 0;
        int completedCount = 0;
        int processIndex = 0;
        ProcessData currentProcess = null;
        int lastEventEndTime = 0; // For merging Gantt events

        while (completedCount < processes.size()) {
            // Add newly arrived processes
            while (processIndex < remainingProcesses.size() && remainingProcesses.get(processIndex).arrivalTime <= currentTime) {
                readyQueue.add(remainingProcesses.get(processIndex));
                processIndex++;
            }

            // Check if preemption is needed
            if (currentProcess != null && !readyQueue.isEmpty() && readyQueue.peek().remainingTime < currentProcess.remainingTime) {
                 // Record Gantt event for the preempted process segment
                 if (currentTime > lastEventEndTime) {
                    ganttChart.add(new GanttEvent(currentProcess.id, lastEventEndTime, currentTime));
                 }
                 readyQueue.add(currentProcess); // Add current back to ready queue
                 currentProcess = null; // Force selection of new shortest job
            }


            // If CPU is idle or current process finished, select a new one
            if (currentProcess == null) {
                if (!readyQueue.isEmpty()) {
                    currentProcess = readyQueue.poll();
                     if (currentProcess.startTime == -1) currentProcess.startTime = currentTime; // Record first start time
                    lastEventEndTime = currentTime; // Start of a new Gantt segment potentially
                } else if (processIndex < remainingProcesses.size()) {
                    // If ready queue empty, advance time to next arrival
                    currentTime = remainingProcesses.get(processIndex).arrivalTime;
                    continue; // Re-check arrivals at new time
                } else {
                    // No processes ready and no more arrivals
                    break; // All done or error
                }
            }

             // Execute one time unit
            if (currentProcess != null) {
                currentProcess.remainingTime--;
                currentTime++;

                // Check if process completed
                if (currentProcess.remainingTime == 0) {
                    currentProcess.calculateMetrics(currentTime);
                    finishedProcesses.add(currentProcess);
                    completedCount++;
                     // Record final Gantt event for this process
                    ganttChart.add(new GanttEvent(currentProcess.id, lastEventEndTime, currentTime));
                    currentProcess = null; // CPU becomes free
                    lastEventEndTime = currentTime; // Reset for potential idle time
                }
            }
        }

        // Merge consecutive Gantt events for the same process
        List<GanttEvent> mergedGantt = mergeGanttEvents(ganttChart);
         return formatProcessResults(finishedProcesses, mergedGantt);
    }

     // 5. Round Robin (RR)
    private String runRR(List<ProcessData> processes, int quantum) {
        Queue<ProcessData> readyQueue = new LinkedList<>();
        List<ProcessData> remainingProcesses = new ArrayList<>(processes);
        List<ProcessData> finishedProcesses = new ArrayList<>();
        List<GanttEvent> ganttChart = new ArrayList<>();
        remainingProcesses.sort(Comparator.comparingInt(p -> p.arrivalTime));

        int currentTime = 0;
        int processIndex = 0;
        int completedCount = 0;
        int lastEventEndTime = 0;

        // Initial population of ready queue at time 0 (or first arrival)
        while (processIndex < remainingProcesses.size() && remainingProcesses.get(processIndex).arrivalTime <= currentTime) {
             readyQueue.offer(remainingProcesses.get(processIndex));
             processIndex++;
        }
        if (readyQueue.isEmpty() && processIndex < remainingProcesses.size()) {
            currentTime = remainingProcesses.get(processIndex).arrivalTime;
             while (processIndex < remainingProcesses.size() && remainingProcesses.get(processIndex).arrivalTime <= currentTime) {
                 readyQueue.offer(remainingProcesses.get(processIndex));
                 processIndex++;
             }
        }


        while (completedCount < processes.size()) {
            if (readyQueue.isEmpty()) {
                 // If ready queue is empty, advance time to the next arrival
                if (processIndex < remainingProcesses.size()) {
                     currentTime = remainingProcesses.get(processIndex).arrivalTime;
                     // Add newly arrived processes
                     while (processIndex < remainingProcesses.size() && remainingProcesses.get(processIndex).arrivalTime <= currentTime) {
                         readyQueue.offer(remainingProcesses.get(processIndex));
                         processIndex++;
                     }
                } else {
                     // No more processes waiting or arriving
                     break;
                }
                 if (readyQueue.isEmpty()) continue; // Still empty after time jump? Skip to next cycle.
            }

            ProcessData currentProcess = readyQueue.poll();
             if (currentProcess.startTime == -1) currentProcess.startTime = currentTime;
            int startTime = currentTime;

            int timeSlice = Math.min(quantum, currentProcess.remainingTime);
            currentProcess.remainingTime -= timeSlice;
            currentTime += timeSlice;

            // Add newly arrived processes during the timeslice execution
            while (processIndex < remainingProcesses.size() && remainingProcesses.get(processIndex).arrivalTime <= currentTime) {
                readyQueue.offer(remainingProcesses.get(processIndex));
                processIndex++;
            }

             ganttChart.add(new GanttEvent(currentProcess.id, startTime, currentTime));

            if (currentProcess.remainingTime == 0) {
                // Process finished
                currentProcess.calculateMetrics(currentTime);
                finishedProcesses.add(currentProcess);
                completedCount++;
            } else {
                // Process not finished, add back to the end of the queue
                readyQueue.offer(currentProcess);
            }
        }
         // RR Gantt chart usually doesn't need merging like preemptive
         return formatProcessResults(finishedProcesses, ganttChart);
    }


    // 6. Priority Scheduling - Preemptive (Lower number means higher priority)
    private String runPriority_P(List<ProcessData> processes) {
        PriorityQueue<ProcessData> readyQueue = new PriorityQueue<>(Comparator.comparingInt(p -> p.priority));
        List<ProcessData> remainingProcesses = new ArrayList<>(processes);
        List<ProcessData> finishedProcesses = new ArrayList<>();
        List<GanttEvent> ganttChart = new ArrayList<>();
        remainingProcesses.sort(Comparator.comparingInt(p -> p.arrivalTime));

        int currentTime = 0;
        int completedCount = 0;
        int processIndex = 0;
        ProcessData currentProcess = null;
        int lastEventEndTime = 0;

        while (completedCount < processes.size()) {
            // Add newly arrived processes
            while (processIndex < remainingProcesses.size() && remainingProcesses.get(processIndex).arrivalTime <= currentTime) {
                readyQueue.add(remainingProcesses.get(processIndex));
                processIndex++;
            }

             // Check for preemption: If a higher priority process arrives or is waiting
            if (currentProcess != null && !readyQueue.isEmpty() && readyQueue.peek().priority < currentProcess.priority) {
                 if (currentTime > lastEventEndTime) {
                    ganttChart.add(new GanttEvent(currentProcess.id, lastEventEndTime, currentTime));
                 }
                 readyQueue.add(currentProcess); // Add current back
                 currentProcess = null; // Force check for highest priority
            }


            // If CPU is idle or current process finished/preempted, select a new one
            if (currentProcess == null) {
                if (!readyQueue.isEmpty()) {
                    currentProcess = readyQueue.poll();
                     if (currentProcess.startTime == -1) currentProcess.startTime = currentTime;
                    lastEventEndTime = currentTime;
                } else if (processIndex < remainingProcesses.size()) {
                    currentTime = remainingProcesses.get(processIndex).arrivalTime;
                    continue;
                } else {
                    break;
                }
            }

            // Execute one time unit
            if (currentProcess != null) {
                currentProcess.remainingTime--;
                currentTime++;

                // Check if process completed
                if (currentProcess.remainingTime == 0) {
                    currentProcess.calculateMetrics(currentTime);
                    finishedProcesses.add(currentProcess);
                    completedCount++;
                    ganttChart.add(new GanttEvent(currentProcess.id, lastEventEndTime, currentTime));
                    currentProcess = null;
                    lastEventEndTime = currentTime;
                }
            }
        }

        List<GanttEvent> mergedGantt = mergeGanttEvents(ganttChart);
         return formatProcessResults(finishedProcesses, mergedGantt);
    }

      // Helper to merge consecutive Gantt events for the same process
    private List<GanttEvent> mergeGanttEvents(List<GanttEvent> originalGantt) {
        if (originalGantt.isEmpty()) return originalGantt;

        List<GanttEvent> merged = new ArrayList<>();
        GanttEvent currentMerged = null;

        for (GanttEvent event : originalGantt) {
            if (currentMerged == null) {
                currentMerged = new GanttEvent(event.processId, event.startTime, event.endTime);
            } else if (event.processId.equals(currentMerged.processId) && event.startTime == currentMerged.endTime) {
                // Merge consecutive blocks of the same process
                currentMerged.endTime = event.endTime;
            } else {
                // Different process or non-contiguous block, add the previous merged event and start a new one
                merged.add(currentMerged);
                currentMerged = new GanttEvent(event.processId, event.startTime, event.endTime);
            }
        }
         if (currentMerged != null) {
             merged.add(currentMerged); // Add the last merged event
         }
        return merged;
    }


    // == Disk Scheduling Algorithms ==

    // Helper to format disk results
    private String formatDiskResults(List<Integer> sequence, int totalMovement, int headStart) {
        StringBuilder sb = new StringBuilder();
        sb.append("Initial Head Position: ").append(headStart).append("\n");
        sb.append("Seek Sequence: ");
        sb.append(headStart); // Start from the initial position
        for (int track : sequence) {
            sb.append(" -> ").append(track);
        }
        sb.append("\n");
        sb.append("Total Head Movement: ").append(totalMovement).append(" cylinders\n");
        return sb.toString();
    }

    // 1. Disk FCFS
    private String runDiskFCFS(List<Integer> queue, int headStart) {
        int currentHead = headStart;
        int totalMovement = 0;
        List<Integer> sequence = new ArrayList<>();

        for (int request : queue) {
            sequence.add(request);
            totalMovement += Math.abs(request - currentHead);
            currentHead = request;
        }
        return formatDiskResults(sequence, totalMovement, headStart);
    }

    // 2. Disk SSTF
    private String runSSTF(List<Integer> queue, int headStart) {
        int currentHead = headStart;
        int totalMovement = 0;
        List<Integer> sequence = new ArrayList<>();
        List<Integer> remainingQueue = new ArrayList<>(queue);

        while (!remainingQueue.isEmpty()) {
            int shortestSeek = Integer.MAX_VALUE;
            int nextRequestIndex = -1;

            // Find the request with the shortest seek time from the current head
            for (int i = 0; i < remainingQueue.size(); i++) {
                int seek = Math.abs(remainingQueue.get(i) - currentHead);
                if (seek < shortestSeek) {
                    shortestSeek = seek;
                    nextRequestIndex = i;
                }
            }

            int nextRequest = remainingQueue.remove(nextRequestIndex);
            sequence.add(nextRequest);
            totalMovement += shortestSeek;
            currentHead = nextRequest;
        }
        return formatDiskResults(sequence, totalMovement, headStart);
    }

    // 3. Disk SCAN (Elevator)
    private String runSCAN(List<Integer> queue, int headStart, int diskSize, boolean movingRight) {
        int currentHead = headStart;
        int totalMovement = 0;
        List<Integer> sequence = new ArrayList<>();
        List<Integer> remainingQueue = new ArrayList<>(queue);
        remainingQueue.sort(Comparator.naturalOrder()); // Sort requests numerically

        List<Integer> left = new ArrayList<>();
        List<Integer> right = new ArrayList<>();

        // Partition requests based on current head position
        for (int req : remainingQueue) {
            if (req < currentHead) {
                left.add(req);
            } else {
                right.add(req);
            }
        }

        // Process requests based on direction
        int run = 2; // Max 2 passes (one in each direction if needed)
        while (run-- > 0) {
             if (movingRight) {
                // Service requests to the right
                for (int req : right) {
                    sequence.add(req);
                    totalMovement += Math.abs(req - currentHead);
                    currentHead = req;
                }
                 right.clear(); // Clear served requests

                 // If there are requests on the left or it's the first pass, move to the end
                if (!left.isEmpty() || run == 1) {
                    if (currentHead != diskSize) { // Avoid adding movement if already at the end
                         totalMovement += Math.abs(diskSize - currentHead);
                         currentHead = diskSize;
                         // Only add the endpoint to sequence if we actually moved there and served something on the way or plan to come back
                         // sequence.add(diskSize); // Optional: Add endpoint to sequence
                    }
                }
             } else { // movingLeft
                // Service requests to the left (in descending order)
                Collections.reverse(left); // Process from nearest to farthest left
                for (int req : left) {
                     sequence.add(req);
                     totalMovement += Math.abs(req - currentHead);
                     currentHead = req;
                }
                left.clear();

                // If there are requests on the right or it's the first pass, move to the beginning
                if (!right.isEmpty() || run == 1) {
                    if (currentHead != 0) { // Avoid adding movement if already at 0
                        totalMovement += Math.abs(0 - currentHead);
                        currentHead = 0;
                        // sequence.add(0); // Optional: Add endpoint to sequence
                    }
                }
            }
            movingRight = !movingRight; // Reverse direction
        }

        return formatDiskResults(sequence, totalMovement, headStart);
    }


     // 4. Disk C-SCAN (Circular SCAN)
    private String runCSCAN(List<Integer> queue, int headStart, int diskSize, boolean movingRight) {
        int currentHead = headStart;
        int totalMovement = 0;
        List<Integer> sequence = new ArrayList<>();
        List<Integer> remainingQueue = new ArrayList<>(queue);
        remainingQueue.sort(Comparator.naturalOrder());

        List<Integer> left = new ArrayList<>();
        List<Integer> right = new ArrayList<>();

        for (int req : remainingQueue) {
            if (req < currentHead) {
                left.add(req);
            } else {
                right.add(req);
            }
        }

         if (movingRight) {
             // Service right
             for (int req : right) {
                 sequence.add(req);
                 totalMovement += Math.abs(req - currentHead);
                 currentHead = req;
             }
             // Jump to the end if necessary
             if (!left.isEmpty() || currentHead != diskSize) {
                 if (currentHead != diskSize) {
                     totalMovement += Math.abs(diskSize - currentHead);
                     currentHead = diskSize;
                     // sequence.add(diskSize); // Optionally track endpoints
                 }
                 // Jump to the beginning
                 if (!left.isEmpty()) {
                      totalMovement += diskSize; // Full jump from end to beginning
                      currentHead = 0;
                      // sequence.add(0); // Optionally track endpoints
                      // Service left (now treated as right from 0)
                      for (int req : left) {
                         sequence.add(req);
                         totalMovement += Math.abs(req - currentHead);
                         currentHead = req;
                      }
                 }
             }
         } else { // movingLeft initially (less common variant)
             // Service left (descending)
             Collections.reverse(left);
             for (int req : left) {
                  sequence.add(req);
                  totalMovement += Math.abs(req - currentHead);
                  currentHead = req;
             }
             // Jump to beginning if necessary
              if (!right.isEmpty() || currentHead != 0) {
                  if (currentHead != 0) {
                     totalMovement += Math.abs(0 - currentHead);
                     currentHead = 0;
                     // sequence.add(0);
                  }
                  // Jump to end
                  if (!right.isEmpty()) {
                      totalMovement += diskSize; // Full jump
                      currentHead = diskSize;
                      // sequence.add(diskSize);
                      // Service right (now treated as left from diskSize, descending)
                      Collections.reverse(right); // Sort descending relative to end
                      for (int req : right) {
                         sequence.add(req);
                         totalMovement += Math.abs(req - currentHead);
                         currentHead = req;
                      }
                  }
             }
         }

        return formatDiskResults(sequence, totalMovement, headStart);
    }


    // 5. Disk LOOK
    private String runLOOK(List<Integer> queue, int headStart, boolean movingRight) {
        int currentHead = headStart;
        int totalMovement = 0;
        List<Integer> sequence = new ArrayList<>();
        List<Integer> remainingQueue = new ArrayList<>(queue);
        remainingQueue.sort(Comparator.naturalOrder());

        List<Integer> left = new ArrayList<>();
        List<Integer> right = new ArrayList<>();

        for (int req : remainingQueue) {
            if (req < currentHead) {
                left.add(req);
            } else {
                right.add(req);
            }
        }

        int run = 2;
        while (run-- > 0 && (!left.isEmpty() || !right.isEmpty())) { // Stop if both sides are empty
            if (movingRight) {
                // Service right
                for (int req : right) {
                    sequence.add(req);
                    totalMovement += Math.abs(req - currentHead);
                    currentHead = req;
                }
                right.clear(); // Mark as served
            } else { // movingLeft
                // Service left (descending)
                Collections.reverse(left);
                for (int req : left) {
                    sequence.add(req);
                    totalMovement += Math.abs(req - currentHead);
                    currentHead = req;
                }
                left.clear();
            }
            movingRight = !movingRight; // Reverse direction only if needed
             // Only reverse if the other side still has requests
            if ((movingRight && right.isEmpty()) || (!movingRight && left.isEmpty())) {
                break; // No need to reverse if the target direction is empty
            }
        }
        return formatDiskResults(sequence, totalMovement, headStart);
    }

    // 6. Disk C-LOOK (Circular LOOK)
     private String runCLOOK(List<Integer> queue, int headStart, boolean movingRight) {
        int currentHead = headStart;
        int totalMovement = 0;
        List<Integer> sequence = new ArrayList<>();
        List<Integer> remainingQueue = new ArrayList<>(queue);
        remainingQueue.sort(Comparator.naturalOrder());

        List<Integer> left = new ArrayList<>();
        List<Integer> right = new ArrayList<>();

        for (int req : remainingQueue) {
            if (req < currentHead) {
                left.add(req);
            } else {
                right.add(req);
            }
        }


         if (movingRight) {
             // Service right
             for (int req : right) {
                 sequence.add(req);
                 totalMovement += Math.abs(req - currentHead);
                 currentHead = req;
             }
             // Jump to smallest request if needed
             if (!left.isEmpty()) {
                 // Find the smallest request overall (first element of sorted 'left')
                  if (!left.isEmpty()) {
                      int jumpTo = left.get(0); // Jump to the minimum element on the other side
                      totalMovement += Math.abs(jumpTo - currentHead); // Jump
                      currentHead = jumpTo;
                      sequence.add(currentHead);

                       // Service remaining left requests (now treated as right from the new head)
                       // Remove the jumped-to element as it's now served
                      left.remove(0);
                      for(int req : left) {
                          sequence.add(req);
                          totalMovement += Math.abs(req-currentHead);
                          currentHead = req;
                      }
                  }
             }
         } else { // movingLeft initially
             // Service left (descending)
              Collections.reverse(left);
             for (int req : left) {
                  sequence.add(req);
                  totalMovement += Math.abs(req - currentHead);
                  currentHead = req;
             }
             // Jump to largest request if needed
              if (!right.isEmpty()) {
                  // Find largest request overall (last element of sorted 'right')
                  if (!right.isEmpty()) {
                       int jumpTo = right.get(right.size()-1);
                      totalMovement += Math.abs(jumpTo - currentHead); // Jump
                      currentHead = jumpTo;
                      sequence.add(currentHead);

                      // Service remaining right requests (now treated as left from the new head - descending)
                      right.remove(right.size()-1); // Remove jumped-to
                      Collections.reverse(right); // Ensure they are processed towards lower numbers now
                      for(int req : right) {
                           sequence.add(req);
                           totalMovement += Math.abs(req-currentHead);
                           currentHead = req;
                      }
                  }
             }
         }


        return formatDiskResults(sequence, totalMovement, headStart);
    }

    // == Page Replacement Algorithms ==

     // Helper to format page replacement results
    private String formatPageResults(int pageFaults, int pageHits, List<Integer> refString, List<String> history) {
        StringBuilder sb = new StringBuilder();
        sb.append("Reference String: ").append(refString.toString()).append("\n");
        sb.append("Page Faults: ").append(pageFaults).append("\n");
        sb.append("Page Hits: ").append(pageHits).append("\n");
        double faultRate = (double) pageFaults / refString.size();
        sb.append(String.format("Page Fault Rate: %.2f%%\n\n", faultRate * 100));

         sb.append("Step-by-step Frames:\n");
         for (String step : history) {
             sb.append(step).append("\n");
         }

        return sb.toString();
    }

    // 1. Page FIFO
    private String runFIFO(List<Integer> refString, int capacity) {
        Queue<Integer> frameQueue = new LinkedList<>(); // Tracks insertion order
        Set<Integer> frameSet = new HashSet<>(capacity);   // For quick checking of presence
        int pageFaults = 0;
        int pageHits = 0;
         List<String> history = new ArrayList<>();

        for (int page : refString) {
            String currentStep = String.format("Ref: %-3d Frames: ", page);
            if (frameSet.contains(page)) {
                pageHits++;
                 currentStep += frameQueue.toString() + " (Hit)";
            } else {
                pageFaults++;
                if (frameSet.size() < capacity) {
                    // Still space available
                    frameSet.add(page);
                    frameQueue.offer(page);
                     currentStep += frameQueue.toString() + " (Fault - Free)";
                } else {
                    // Need to replace
                    int removedPage = frameQueue.poll(); // Get the oldest page (front of queue)
                    frameSet.remove(removedPage);      // Remove from set
                    frameSet.add(page);                // Add new page to set
                    frameQueue.offer(page);            // Add new page to queue
                    currentStep += frameQueue.toString() + String.format(" (Fault - Replaced %d)", removedPage);
                }
            }
             history.add(currentStep);
        }
        return formatPageResults(pageFaults, pageHits, refString, history);
    }

    // 2. Page LRU (Least Recently Used)
    private String runLRU(List<Integer> refString, int capacity) {
        // Using LinkedHashSet to maintain insertion order AND allow quick removal/re-insertion
        // The element accessed most recently is moved to the end (most-recently-used)
        LinkedHashSet<Integer> frames = new LinkedHashSet<>(capacity);
        int pageFaults = 0;
        int pageHits = 0;
        List<String> history = new ArrayList<>();

        for (int page : refString) {
             String currentStep = String.format("Ref: %-3d Frames: ", page);
            if (frames.contains(page)) {
                pageHits++;
                // Move the accessed page to the end (most recently used)
                frames.remove(page);
                frames.add(page);
                 currentStep += new ArrayList<>(frames).toString() + " (Hit)"; // Convert to list for stable order view
            } else {
                pageFaults++;
                if (frames.size() == capacity) {
                    // Remove the least recently used (the first element in the LinkedHashSet iteration order)
                    Iterator<Integer> it = frames.iterator();
                    int removedPage = it.next();
                    it.remove(); // Remove the LRU element
                    frames.add(page); // Add the new page (becomes MRU)
                     currentStep += new ArrayList<>(frames).toString() + String.format(" (Fault - Replaced %d)", removedPage);
                } else {
                    // Still space available
                    frames.add(page); // Add the new page (becomes MRU)
                     currentStep += new ArrayList<>(frames).toString() + " (Fault - Free)";
                }
            }
             history.add(currentStep);
        }
        return formatPageResults(pageFaults, pageHits, refString, history);
    }

    // 3. Page Optimal
    private String runOptimal(List<Integer> refString, int capacity) {
        Set<Integer> frames = new HashSet<>(capacity);
        List<Integer> frameList = new ArrayList<>(capacity); // To know *which* specific page is in a slot
        int pageFaults = 0;
        int pageHits = 0;
        List<String> history = new ArrayList<>();

        for (int i = 0; i < refString.size(); i++) {
            int page = refString.get(i);
             String currentStep = String.format("Ref: %-3d Frames: ", page);

            if (frames.contains(page)) {
                pageHits++;
                 currentStep += frameList.toString() + " (Hit)";
            } else {
                pageFaults++;
                if (frames.size() < capacity) {
                    // Still space available
                    frames.add(page);
                    frameList.add(page);
                     currentStep += frameList.toString() + " (Fault - Free)";
                } else {
                    // Need to replace - find the page in frames used farthest in the future
                    int pageToReplace = -1;
                    int farthestUse = -1;

                    for (int framePage : frameList) {
                        int nextUse = Integer.MAX_VALUE; // Assume not used again initially
                        // Look for the next occurrence of framePage in the remaining reference string
                        for (int j = i + 1; j < refString.size(); j++) {
                            if (refString.get(j) == framePage) {
                                nextUse = j;
                                break; // Found the nearest future use
                            }
                        }
                        // If this page is used farther in the future than the current farthest, update
                        if (nextUse > farthestUse) {
                            farthestUse = nextUse;
                            pageToReplace = framePage;
                        }
                    }

                    // Find the index of the page to replace in our frameList
                    int replaceIndex = frameList.indexOf(pageToReplace);
                    frames.remove(pageToReplace);
                    frameList.set(replaceIndex, page); // Replace in the list
                    frames.add(page);                  // Add new page to the set
                    currentStep += frameList.toString() + String.format(" (Fault - Replaced %d)", pageToReplace);
                }
            }
             history.add(currentStep);
        }
        return formatPageResults(pageFaults, pageHits, refString, history);
    }


    // --- Main Method ---
    public static void main(String[] args) {
        // Best practice: Run Swing GUI code on the Event Dispatch Thread (EDT)
        SwingUtilities.invokeLater(() -> new OsSimulatorGUI());
    }
}