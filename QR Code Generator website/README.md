# URL to QR Code Generator

A simple, elegant web application that converts URLs into QR codes with the ability to display and save them as images.

## Features

🔗 **URL Input**: Enter any URL and automatically validate it  
📱 **QR Code Generation**: Creates high-quality QR codes using QRCode.js  
💾 **Save as Image**: Download QR codes as PNG files  
📋 **Copy to Clipboard**: Copy QR code images or URLs to clipboard  
🎨 **Responsive Design**: Works perfectly on desktop and mobile devices  
⌨️ **Keyboard Shortcuts**: Press Enter to generate, Escape to clear  

## How to Use

1. **Enter a URL** in the input field (e.g., `example.com` or `https://example.com`)
2. **Click "Generate QR Code"** or press Enter
3. **View your QR code** - it will appear below the input
4. **Save or copy** the QR code using the buttons provided

## Getting Started

### Option 1: Open directly in browser
Simply open `index.html` in your web browser - no server required!

### Option 2: Local development server
If you want to run a local server:

```bash
# Using Python 3
python -m http.server 8000

# Using Node.js (if you have it installed)
npx serve .

# Using PHP
php -S localhost:8000
```

Then open `http://localhost:8000` in your browser.

## Technical Details

- **Frontend**: HTML5, CSS3, JavaScript (ES6+)
- **QR Code Library**: QRCode.js (loaded via CDN)
- **No backend required** - runs entirely in the browser
- **Mobile responsive** design
- **Error handling** for invalid URLs
- **Accessibility features** included

## Browser Compatibility

- ✅ Chrome/Chromium (recommended)
- ✅ Firefox
- ✅ Safari
- ✅ Edge
- ℹ️ Internet Explorer not supported

## File Structure

```
├── index.html          # Main HTML page
├── style.css           # CSS styling
├── script.js           # JavaScript functionality
└── README.md           # This file
```

## Keyboard Shortcuts

- **Enter**: Generate QR code
- **Escape**: Clear all
- **Ctrl/Cmd + Enter**: Generate QR code (from anywhere)

## Features in Detail

### URL Validation
- Automatically adds `https://` if no protocol is specified
- Validates URL format before generating QR code
- Shows clear error messages for invalid URLs

### QR Code Generation
- High-quality 256x256 pixel QR codes
- Medium error correction level for reliability
- Clean, professional appearance with rounded corners

### Download & Copy
- Save QR codes as PNG images with timestamp
- Copy QR code images to clipboard (modern browsers)
- Fallback to copy URL text for older browsers

### User Experience
- Smooth animations and transitions
- Loading states for better feedback
- Auto-focus on input field
- Responsive design for all screen sizes

## Customization

You can easily customize the appearance by modifying `style.css`:

- Change colors in the CSS variables
- Adjust QR code size in `script.js` (width/height properties)
- Modify the gradient background
- Update fonts and spacing

## License

This project is open source. Feel free to use, modify, and distribute as needed.

## Contributing

Found a bug or want to add a feature? Feel free to submit issues or pull requests!
