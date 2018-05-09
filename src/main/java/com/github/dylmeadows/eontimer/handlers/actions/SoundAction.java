package com.github.dylmeadows.eontimer.handlers.actions;

import com.github.dylmeadows.common.javafx.node.FxOptionPane;
import com.github.dylmeadows.common.sound.SoundPlayer;
import com.github.dylmeadows.eontimer.EonTimerLogger;
import com.github.dylmeadows.eontimer.reference.resources.SoundResource;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.value.ObservableValue;

import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.UnsupportedAudioFileException;
import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * An audio action capable of playing sounds during a timer countdown.
 * Contains an underlying reference to {@link SoundPlayer}, which is the
 * mechanism for loading and playing sounds.
 */
public class SoundAction implements ICountdownAction {

    /**
     * Underlying audio loader and player.
     */
    private final SoundPlayer player;

    /**
     * The current sound for {@link #player} to play.
     */
    private final ObjectProperty<SoundResource> sound;

    /**
     * Thread to submit tasks to.
     */
    private final ExecutorService executor = Executors.newSingleThreadExecutor();

    /**
     * Instantiates the underlying {@link SoundPlayer}.
     */
    public SoundAction() {
        this.player = new SoundPlayer();
        this.sound = new SimpleObjectProperty<>();
        this.sound.addListener(this::onSoundChange);
    }

    /**
     * Delegate call to {@link SoundPlayer#play()}.
     */
    @Override
    public void action() {
        executor.submit(player::play);
    }

    /**
     * Loads the new sound to the sound player when the sound has changed.
     *
     * @param observable property that changed
     * @param oldValue   old sound
     * @param newValue   new sound
     */
    private void onSoundChange(ObservableValue<? extends SoundResource> observable, SoundResource oldValue, SoundResource newValue) {
        try {
            player.load(newValue);
        } catch (LineUnavailableException | IOException | UnsupportedAudioFileException e) {
            EonTimerLogger.getLogger().error(e);
            FxOptionPane.showExceptionDialog(e);
        }
    }

    /**
     * @return see {@link #sound}
     */
    public SoundResource getSound() {
        return sound.get();
    }

    /**
     * @return see {@link #sound}
     */
    public ObjectProperty<SoundResource> soundProperty() {
        return sound;
    }

    /**
     * @param sound see {@link #sound}
     */
    public void setSound(SoundResource sound) {
        this.sound.set(sound);
    }
}