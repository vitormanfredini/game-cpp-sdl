<?php
function listImages($dir) {
    $rii = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($dir));

    $files = array(); 
    foreach ($rii as $file) {
        if (!$file->isDir()) {
            $files[] = $file->getPathname(); 
        }
    }
    return $files;
}

function outputAsBase64($filePath) {
    $type = pathinfo($filePath, PATHINFO_EXTENSION);
    $data = file_get_contents($filePath);
    return 'data:image/' . $type . ';base64,' . base64_encode($data);
}

$imageDir = '../resources/images';
$imageFile = isset($_GET['file']) ? $_GET['file'] : null;
$frames = isset($_GET['frames']) ? (int)$_GET['frames'] : 12;
$interval = isset($_GET['interval']) ? (int)$_GET['interval'] : 300;
$base64Image = null;
$imageWidth = 0;
$imageHeight = 0;
$frameWidth = 0;

if ($imageFile && file_exists($imageFile)) {
    $base64Image = outputAsBase64($imageFile);
    $imageInfo = getimagesize($imageFile);
    if ($imageInfo) {
        $imageWidth = $imageInfo[0];
        $imageHeight = $imageInfo[1];
        $frameWidth = $imageWidth / $frames;
    }
}

$images = listImages($imageDir);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sprite Animation Viewer</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: #333;
            min-height: 100vh;
            padding: 20px;
        }

        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
            overflow: hidden;
        }

        .header {
            background: linear-gradient(135deg, #4facfe 0%, #00f2fe 100%);
            color: white;
            padding: 30px;
            text-align: center;
        }

        .header h1 {
            font-size: 2.5rem;
            font-weight: 300;
            margin-bottom: 10px;
        }

        .content {
            padding: 30px;
        }

        .preview-section {
            margin-bottom: 40px;
            padding: 20px;
            background: #f8f9fa;
            border-radius: 10px;
            border: 1px solid #e9ecef;
        }

        .preview-section h2 {
            color: #495057;
            margin-bottom: 20px;
            font-size: 1.5rem;
            border-bottom: 2px solid #007bff;
            padding-bottom: 10px;
        }

        .controls {
            background: white;
            padding: 20px;
            border-radius: 8px;
            margin-bottom: 20px;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
        }

        .controls form {
            display: flex;
            align-items: center;
            gap: 15px;
            flex-wrap: wrap;
        }

        .controls label {
            font-weight: 600;
            color: #495057;
        }

        .controls input[type="number"] {
            padding: 10px;
            border: 2px solid #ced4da;
            border-radius: 5px;
            font-size: 16px;
            width: 100px;
            transition: border-color 0.3s ease;
        }

        .controls input[type="number"]:focus {
            outline: none;
            border-color: #007bff;
        }

        .controls button {
            background: linear-gradient(135deg, #28a745, #20c997);
            color: white;
            border: none;
            padding: 12px 24px;
            border-radius: 25px;
            font-size: 16px;
            cursor: pointer;
            transition: all 0.3s ease;
            font-weight: 600;
        }

        .controls button:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(40, 167, 69, 0.4);
        }

        .animation-container {
            display: flex;
            gap: 40px;
            align-items: flex-start;
            flex-wrap: wrap;
        }

        .sprite-preview {
            flex: 1;
            min-width: 200px;
            overflow: visible;
        }

        .sprite-animation-outer {
            display: inline-block;
            border: 5px solid #007bff;
            border-radius: 8px;
            background-color: white;
            margin-bottom: 20px;
            overflow: hidden;
        }

        .sprite-animation-container {
            overflow: hidden;
            display: block;
            position: relative;
        }

        .sprite {
            image-rendering: pixelated;
            background-repeat: no-repeat;
            transform: scale(4);
            transform-origin: top left;
        }

        .static-preview {
            flex: 1;
            min-width: 200px;
        }

        .static-preview img {
            max-width: 100%;
            height: auto;
            border: 5px solid #6c757d;
            border-radius: 8px;
            background-color: white;
            image-rendering: pixelated;
        }

        .file-list {
            background: white;
            border-radius: 10px;
            padding: 20px;
            box-shadow: 0 5px 20px rgba(0, 0, 0, 0.1);
        }

        .file-list h2 {
            color: #495057;
            margin-bottom: 20px;
            font-size: 1.5rem;
            border-bottom: 2px solid #6f42c1;
            padding-bottom: 10px;
        }

        .file-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
            gap: 10px;
        }

        .file-item {
            background: linear-gradient(135deg, #f8f9fa, #e9ecef);
            padding: 12px 16px;
            border-radius: 8px;
            text-decoration: none;
            color: #495057;
            transition: all 0.3s ease;
            border: 2px solid transparent;
            display: block;
        }

        .file-item:hover {
            background: linear-gradient(135deg, #007bff, #0056b3);
            color: white;
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(0, 123, 255, 0.3);
        }

        .file-item.active {
            background: linear-gradient(135deg, #28a745, #20c997);
            color: white;
            border-color: #155724;
        }

        .no-selection {
            text-align: center;
            padding: 40px;
            color: #6c757d;
            font-style: italic;
        }

        .animation-info {
            background: #e7f3ff;
            padding: 15px;
            border-radius: 8px;
            margin-bottom: 20px;
            border-left: 4px solid #007bff;
        }

        .animation-info p {
            margin: 5px 0;
            color: #495057;
        }

        @media (max-width: 768px) {
            .animation-container {
                flex-direction: column;
            }
            
            .file-grid {
                grid-template-columns: 1fr;
            }
            
            .controls form {
                flex-direction: column;
                align-items: stretch;
            }
            
            .controls input[type="number"] {
                width: 100%;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🎮 Sprite Animation Viewer</h1>
            <p>View and animate sprite sheets for your game project</p>
        </div>

        <div class="content">
            <?php if ($base64Image): ?>
                <div class="preview-section">
                    <h2>🎬 Animation Preview</h2>
                    
                    <div class="controls">
                        <form method="GET" action="">
                            <input type="hidden" name="file" value="<?php echo htmlspecialchars($imageFile); ?>">
                            <label for="frames">Number of Frames:</label>
                            <input type="number" id="frames" name="frames" value="<?php echo $frames; ?>" min="1" max="100">
                            <label for="interval">Interval (ms):</label>
                            <input type="number" id="interval" name="interval" value="<?php echo $interval; ?>" min="10" max="5000" step="10">
                            <button type="submit">🔄 Apply</button>
                        </form>
                    </div>

                    <div class="animation-info">
                        <p><strong>File:</strong> <?php echo htmlspecialchars(basename($imageFile)); ?></p>
                        <p><strong>Dimensions:</strong> <?php echo $imageWidth; ?>x<?php echo $imageHeight; ?>px</p>
                        <p><strong>Frames:</strong> <?php echo $frames; ?></p>
                        <p><strong>Frame Size:</strong> <?php echo $frameWidth; ?>x<?php echo $imageHeight; ?>px</p>
                        <p><strong>Animation Speed:</strong> <?php echo $interval; ?>ms per frame</p>
                    </div>

                    <div class="animation-container">
                        <div class="sprite-preview">
                            <h3>Animated Sprite</h3>
                            <div class="sprite-animation-outer">
                                <div class="sprite-animation-container" style="width: <?php echo ($frameWidth * 4); ?>px; height: <?php echo ($imageHeight * 4); ?>px;">
                                    <div class="sprite" id="sprite-animation" style="background-image: url('<?php echo $base64Image; ?>'); width: <?php echo $frameWidth; ?>px; height: <?php echo $imageHeight; ?>px;"></div>
                                </div>
                            </div>
                        </div>
                        
                        <div class="static-preview">
                            <h3>Full Sprite Sheet</h3>
                            <img src="<?php echo $base64Image; ?>" alt="Full Image">
                        </div>
                    </div>
                </div>

                    <script>
                        (function() {
                            var frames = <?php echo $frames; ?>;
                            var frameWidth = <?php echo $frameWidth; ?>;
                            var currentFrame = 0;
                            var sprite = document.getElementById('sprite-animation');
                            
                            setInterval(function() {
                                sprite.style.backgroundPosition = `-${currentFrame * frameWidth}px 0px`;
                                currentFrame = (currentFrame + 1) % frames;
                            }, <?php echo $interval; ?>); // Set the speed of the animation
                        })();
                    </script>
            <?php else: ?>
                <div class="no-selection">
                    <h2>🎯 Select a Sprite Sheet</h2>
                    <p>Choose an image file from the list below to view its animation</p>
                </div>
            <?php endif; ?>

            <div class="file-list">
                <h2>📁 Available Sprite Files</h2>
                <div class="file-grid">
                    <?php foreach ($images as $image): ?>
                        <a href="?file=<?= urlencode($image) ?>" 
                           class="file-item <?= ($imageFile === $image) ? 'active' : '' ?>">
                            <?= htmlspecialchars(basename($image)) ?>
                        </a>
                    <?php endforeach; ?>
                </div>
            </div>
        </div>
    </div>
</body>
</html>
