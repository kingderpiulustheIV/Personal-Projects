#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Function to set up non-blocking input
void setup_terminal() {
    struct termios new_termios;
    tcgetattr(0, &new_termios);
    new_termios.c_lflag &= ~ICANON;
    new_termios.c_lflag &= ~ECHO;
    new_termios.c_lflag &= ~ISIG;  // Disable signal generation (Ctrl+C, etc)
    new_termios.c_iflag &= ~(IXON | IXOFF | IXANY);  // Disable flow control
    new_termios.c_iflag &= ~(INLCR | IGNCR | ICRNL);  // Disable line ending conversions
    tcsetattr(0, TCSANOW, &new_termios);
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    
    // Comprehensive mouse disabling - disable ALL mouse functionality
    printf("\x1b[?1000l");  // Disable mouse click reporting (X10 mode)
    printf("\x1b[?1001l");  // Disable mouse highlight reporting
    printf("\x1b[?1002l");  // Disable mouse drag reporting (button event tracking)
    printf("\x1b[?1003l");  // Disable mouse move reporting (any event tracking)
    printf("\x1b[?1004l");  // Disable focus reporting
    printf("\x1b[?1005l");  // Disable UTF-8 mouse mode
    printf("\x1b[?1006l");  // Disable SGR mouse mode
    printf("\x1b[?1015l");  // Disable urxvt mouse mode
    printf("\x1b[?25l");    // Hide cursor to prevent mouse interaction
    printf("\x1b[?47h");    // Use alternate screen buffer
    printf("\x1b[?1049h");  // Save cursor and use alternate screen
    fflush(stdout);
}

// Function to check for key press without blocking
char get_key() {
    char c;
    static char escape_buffer[8];
    static int escape_pos = 0;
    
    if (read(0, &c, 1) > 0) {
        // Handle escape sequences (mouse input, arrow keys, etc.)
        if (c == '\x1b') {  // ESC character
            escape_pos = 0;
            escape_buffer[escape_pos++] = c;
            return 0;  // Ignore escape sequences
        } else if (escape_pos > 0 && escape_pos < 7) {
            escape_buffer[escape_pos++] = c;
            // Common escape sequence endings
            if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '~' || c == 'M') {
                escape_pos = 0;  // Reset buffer
                return 0;  // Ignore the complete escape sequence
            }
            return 0;  // Still building escape sequence
        } else {
            escape_pos = 0;  // Reset if buffer overflow
            // Only return valid control keys
            if (c == 'p' || c == 'P' || c == 'q' || c == 'Q') {
                return c;
            }
            return 0;  // Ignore all other keys
        }
    }
    return 0;
}

int main() {
    float A = 0, B = 0;
    int paused = 0;
    float t, phi;
    int k;
    float z[1760];
    char b[1760];
    setup_terminal();
    printf("\x1b[2J");
    for(;;) {
        // Check for key press
        char key = get_key();
        if (key == 'p' || key == 'P') {
            paused = !paused;
        } else if (key == 'q' || key == 'Q') {
            break;  // Exit on 'q'
        }
        memset(b,32,1760);
        memset(z,0,7040);
        
        // Create figure 8 using two connected toroids
        for(t = 0; t < 6.28; t += 0.07) {
            for(phi = 0; phi < 6.28; phi += 0.15) {
                float cos_t = cos(t);
                float sin_t = sin(t);
                float cos_phi = cos(phi);
                float sin_phi = sin(phi);
                
                // Create figure 8 
                float R1 = 1.5;  // Major radius 
                float r1 = 0.5;  // Tube thickness
                
                // Upper toroid centered at (0, 1.25)
                float center1_x = 0;
                float center1_y = 1.25;  
                
                // Lower toroid  centered at (0, -1.25)
                float center2_x = 0;
                float center2_y = -1.25;  
                
                // Generate points for upper toroid
                float x1 = (R1 + r1 * cos_phi) * cos_t + center1_x;
                float y1 = (R1 + r1 * cos_phi) * sin_t + center1_y;  
                float z1 = r1 * sin_phi;
                
                // Generate points for lower toroid (rotated 180 degrees)
                float x2 = (R1 + r1 * cos_phi) * cos(t + 3.14159) + center2_x;
                float y2 = (R1 + r1 * cos_phi) * sin(t + 3.14159) + center2_y;
                float z2 = r1 * sin_phi;
                
                // Add center connection piece for toroids
                float connect_size = 0.3;
                float x3 = connect_size * cos_phi;
                float y3 = connect_size * sin_t * 0.2;  
                float z3 = connect_size * sin_phi;
                
                // Process both circles plus center connection
                for(int circle = 0; circle < 3; circle++) {
                    float x, y, z_coord;
                    if(circle == 0) {
                        x = x1; y = y1; z_coord = z1;
                    } else if(circle == 1) {
                        x = x2; y = y2; z_coord = z2;
                    } else {
                        x = x3; y = y3; z_coord = z3;
                    }
                    
                    // Apply rotation transformations
                    float cos_A = cos(A), sin_A = sin(A);
                    float cos_B = cos(B), sin_B = sin(B);
                    
                    // Rotate around Y axis (A)
                    float temp_x = cos_A * x + sin_A * z_coord;
                    float temp_z = -sin_A * x + cos_A * z_coord;
                    
                    // Rotate around X axis (B)
                    float final_x = temp_x;
                    float final_y = cos_B * y - sin_B * temp_z;
                    float final_z = sin_B * y + cos_B * temp_z;
                    
                    // Perspective projection 
                    float depth = final_z + 5;
                    if(depth > 0) {
                        int screen_x = (int)(40 + 25 * final_x / depth);  
                        int screen_y = (int)(11 + 10 * final_y / depth);   
                        int o = screen_x + 80 * screen_y;
                        
                        // Calculate surface normal for torus
                        float surface_normal_x = cos_t * cos_phi;
                        float surface_normal_y = sin_t * cos_phi;
                        float surface_normal_z = sin_phi;
                        
                        // Apply same rotations to normal
                        float norm_temp_x = cos_A * surface_normal_x + sin_A * surface_normal_z;
                        float norm_temp_z = -sin_A * surface_normal_x + cos_A * surface_normal_z;
                        float norm_final_x = norm_temp_x;
                        float norm_final_y = cos_B * surface_normal_y - sin_B * norm_temp_z;
                        float norm_final_z = sin_B * surface_normal_y + cos_B * norm_temp_z;
                        
                        // Light direction (from top-left-front)
                        float light_dot = norm_final_x * 0.6 + norm_final_y * 0.6 + norm_final_z * 0.5;
                        int N = (int)(8 * (light_dot + 1));
                        if(N < 0) N = 0;
                        if(N > 11) N = 11;
                        
                        if(22 > screen_y && screen_y > 0 && screen_x > 0 && 80 > screen_x && depth > z[o]) {
                            z[o] = depth;
                            b[o] = ".,-~:;=!*#$@"[N];
                        }
                    }
                }
            }
        }
        
        // Clear screen and display controls at the top
        printf("\x1b[2J\x1b[H");  // Clear screen and move to top-left
        if (paused) {
            printf("PAUSED - Press 'P' to resume, 'Q' to quit\n");
        } else {
            printf("Press 'P' to pause, 'Q' to quit\n");
        }
        
        // Display the figure 8 starting from line 2
        for(k = 0; k < 1761; k++) {
            putchar(k % 80 ? b[k] : 10);
        }
        
        // Only update rotation when not paused
        if (!paused) {
            A += 0.08;  // Doubled the rotation speed (was 0.04)
            B += 0.04;  // Doubled the rotation speed (was 0.02)
        }
        
        usleep(30000);
    }
    
    // Restore terminal settings
    struct termios restore_termios;
    tcgetattr(0, &restore_termios);
    restore_termios.c_lflag |= ICANON;
    restore_termios.c_lflag |= ECHO;
    restore_termios.c_lflag |= ISIG;  // Re-enable signal generation
    restore_termios.c_iflag |= (IXON | IXOFF | IXANY);  // Re-enable flow control
    restore_termios.c_iflag |= (INLCR | IGNCR | ICRNL);  // Re-enable line endings
    tcsetattr(0, TCSANOW, &restore_termios);
    
    // Restore all mouse functionality and terminal state
    printf("\x1b[?1049l");  // Restore cursor and exit alternate screen
    printf("\x1b[?47l");    // Exit alternate screen buffer
    printf("\x1b[?25h");    // Show cursor
    printf("\x1b[?1000h");  // Re-enable mouse click reporting
    printf("\x1b[?1001h");  // Re-enable mouse highlight reporting
    printf("\x1b[?1002h");  // Re-enable mouse drag reporting
    printf("\x1b[?1003h");  // Re-enable mouse move reporting
    printf("\x1b[?1004h");  // Re-enable focus reporting
    printf("\x1b[?1005h");  // Re-enable UTF-8 mouse mode
    printf("\x1b[?1006h");  // Re-enable SGR mouse mode
    printf("\x1b[?1015h");  // Re-enable urxvt mouse mode
    
    printf("\x1b[2J\x1b[H");  // Clear screen and move cursor to top
    fflush(stdout);
    return 0;
}
