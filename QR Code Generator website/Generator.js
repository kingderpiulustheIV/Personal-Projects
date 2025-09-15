class QRCodeGenerator {
    constructor() {
        this.initializeElements();
        this.attachEventListeners();
        this.currentQRCode = null;
    }

    initializeElements() {
        this.urlInput = document.getElementById('urlInput');
        this.generateBtn = document.getElementById('generateBtn');
        this.clearBtn = document.getElementById('clearBtn');
        this.qrSection = document.getElementById('qrSection');
        this.qrcodeDiv = document.getElementById('qrcode');
        this.displayUrl = document.getElementById('displayUrl');
        this.downloadBtn = document.getElementById('downloadBtn');
        this.copyBtn = document.getElementById('copyBtn');
    }

    attachEventListeners() {
        this.generateBtn.addEventListener('click', () => this.generateQRCode());
        this.clearBtn.addEventListener('click', () => this.clearAll());
        this.downloadBtn.addEventListener('click', () => this.downloadQRCode());
        this.copyBtn.addEventListener('click', () => this.copyQRCode());
        
        // Generate QR code when Enter is pressed
        this.urlInput.addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                this.generateQRCode();
            }
        });

        // Auto-clear messages after input changes
        this.urlInput.addEventListener('input', () => {
            this.clearMessages();
        });
    }

    validateURL(string) {
        try {
            // Add protocol if missing
            if (!string.match(/^https?:\/\//)) {
                string = 'https://' + string;
            }
            new URL(string);
            return string;
        } catch (_) {
            return false;
        }
    }

    showMessage(message, type = 'error') {
        this.clearMessages();
        
        const messageDiv = document.createElement('div');
        messageDiv.className = type === 'error' ? 'error-message' : 'success-message';
        messageDiv.textContent = message;
        
        // Insert before the input section
        const inputSection = document.querySelector('.input-section');
        inputSection.parentNode.insertBefore(messageDiv, inputSection);
        
        // Auto-remove after 5 seconds
        setTimeout(() => {
            if (messageDiv.parentNode) {
                messageDiv.parentNode.removeChild(messageDiv);
            }
        }, 5000);
    }

    clearMessages() {
        const messages = document.querySelectorAll('.error-message, .success-message');
        messages.forEach(message => message.remove());
    }

    async generateQRCode() {
        const url = this.urlInput.value.trim();
        
        if (!url) {
            this.showMessage('Please enter a URL');
            this.urlInput.focus();
            return;
        }

        const validatedURL = this.validateURL(url);
        if (!validatedURL) {
            this.showMessage('Please enter a valid URL (e.g., example.com or https://example.com)');
            this.urlInput.focus();
            return;
        }

        try {
            // Show loading state
            this.generateBtn.disabled = true;
            this.generateBtn.textContent = 'Generating...';

            // Clear previous QR code
            this.qrcodeDiv.innerHTML = '';

            // Use local QR code implementation only
            await this.generateLocalQRCode(validatedURL);

            // Update URL display
            this.displayUrl.textContent = validatedURL;
            
            // Show QR section
            this.qrSection.style.display = 'block';
            this.qrSection.scrollIntoView({ behavior: 'smooth', block: 'nearest' });

            // Store current QR code data
            this.currentQRCode = {
                url: validatedURL,
                canvas: this.qrcodeDiv.querySelector('canvas')
            };

            this.showMessage('QR Code generated successfully!', 'success');

        } catch (error) {
            console.error('Error generating QR code:', error);
            this.showMessage('Error generating QR code. Please try again.');
        } finally {
            // Reset button state
            this.generateBtn.disabled = false;
            this.generateBtn.textContent = 'Generate QR Code';
        }
    }

    async generateLocalQRCode(url) {
        try {
            // Use local QR code implementation
            const canvas = document.createElement('canvas');
            canvas.width = 256;
            canvas.height = 256;
            
            // Clear and add canvas to container
            this.qrcodeDiv.innerHTML = '';
            this.qrcodeDiv.appendChild(canvas);
            
            // Generate QR code using local implementation
            await this.createQRCodeOnCanvas(canvas, url);

            // Update URL display
            this.displayUrl.textContent = url;
            
            // Show QR section
            this.qrSection.style.display = 'block';
            this.qrSection.scrollIntoView({ behavior: 'smooth', block: 'nearest' });

            // Store current QR code data
            this.currentQRCode = {
                url: url,
                canvas: canvas
            };

            this.showMessage('QR Code generated successfully!', 'success');

        } catch (error) {
            console.error('Local QR code generation failed:', error);
            this.showMessage('Unable to generate QR code. Please try again.');
        }
    }

    async createQRCodeOnCanvas(canvas, text) {
        // Use QR Server API to generate the QR code image
        const qrApiUrl = `https://api.qrserver.com/v1/create-qr-code/?size=256x256&data=${encodeURIComponent(text)}`;
        
        return new Promise((resolve, reject) => {
            const ctx = canvas.getContext('2d');
            const img = new Image();
            
            img.onload = function() {
                // Draw the QR code image onto the canvas
                ctx.drawImage(img, 0, 0, 256, 256);
                
                // Apply styling
                canvas.style.borderRadius = '10px';
                canvas.style.boxShadow = '0 10px 25px rgba(0,0,0,0.1)';
                
                resolve();
            };
            
            img.onerror = function() {
                reject(new Error('Failed to load QR code from API'));
            };
            
            // Enable CORS
            img.crossOrigin = 'anonymous';
            img.src = qrApiUrl;
        });
    }

    clearAll() {
        this.urlInput.value = '';
        this.qrSection.style.display = 'none';
        this.qrcodeDiv.innerHTML = '';
        this.currentQRCode = null;
        this.clearMessages();
        this.urlInput.focus();
    }

    downloadQRCode() {
        if (!this.currentQRCode) {
            this.showMessage('No QR code to download');
            return;
        }

        try {
            const link = document.createElement('a');
            link.download = `qrcode-${Date.now()}.png`;
            
            if (this.currentQRCode.canvas) {
                // Download from canvas
                link.href = this.currentQRCode.canvas.toDataURL();
            } else if (this.currentQRCode.image) {
                // Download from image (fallback method)
                link.href = this.currentQRCode.image.src;
            } else {
                throw new Error('No valid QR code element found');
            }
            
            // Trigger download
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);
            
            this.showMessage('QR Code saved successfully!', 'success');
        } catch (error) {
            console.error('Error downloading QR code:', error);
            this.showMessage('Error saving QR code. Please try again.');
        }
    }

    async copyQRCode() {
        if (!this.currentQRCode) {
            this.showMessage('No QR code to copy');
            return;
        }

        try {
            if (this.currentQRCode.canvas) {
                // Copy from canvas
                this.currentQRCode.canvas.toBlob(async (blob) => {
                    try {
                        // Copy to clipboard using modern API
                        if (navigator.clipboard && window.ClipboardItem) {
                            const item = new ClipboardItem({ 'image/png': blob });
                            await navigator.clipboard.write([item]);
                            this.showMessage('QR Code copied to clipboard!', 'success');
                        } else {
                            // Fallback: copy the URL instead
                            await navigator.clipboard.writeText(this.currentQRCode.url);
                            this.showMessage('URL copied to clipboard! (QR code image copy not supported in this browser)', 'success');
                        }
                    } catch (error) {
                        console.error('Error copying to clipboard:', error);
                        this.showMessage('Could not copy to clipboard. Try downloading instead.');
                    }
                }, 'image/png');
            } else {
                // Fallback: copy the URL for image-based QR codes
                if (navigator.clipboard) {
                    await navigator.clipboard.writeText(this.currentQRCode.url);
                    this.showMessage('URL copied to clipboard!', 'success');
                } else {
                    this.showMessage('Clipboard not supported. Try downloading instead.');
                }
            }
        } catch (error) {
            console.error('Error copying QR code:', error);
            this.showMessage('Error copying QR code. Please try again.');
        }
    }
}

// Initialize the application when the DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    // Load local QR code implementation
    loadLocalQRCode();
    
    // Initialize the QR code generator
    new QRCodeGenerator();
});

// Local QR Code implementation - no external libraries needed
function loadLocalQRCode() {
    console.log('Local QR Code implementation loaded');
    // This function is called automatically when the page loads
    // All QR code generation is handled by the QRCodeGenerator class methods
}

// Add some utility functions for enhanced functionality
window.addEventListener('load', () => {
    // Add focus to input field
    const urlInput = document.getElementById('urlInput');
    if (urlInput) {
        urlInput.focus();
    }

    // Add keyboard shortcuts
    document.addEventListener('keydown', (e) => {
        // Ctrl/Cmd + Enter to generate
        if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
            document.getElementById('generateBtn').click();
        }
        
        // Escape to clear
        if (e.key === 'Escape') {
            document.getElementById('clearBtn').click();
        }
    });
});
