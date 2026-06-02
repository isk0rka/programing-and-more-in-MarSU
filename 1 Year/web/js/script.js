(() => {
    const SELECTORS = {
        modal: '[data-auth-modal]',
        openButton: '.js-open-auth',
        closeButton: '[data-auth-close]',
        switchButton: '[data-auth-switch]',
        form: '.auth-form',
        firstInput: '.auth-input',
        feedback: '.auth-feedback'
    };

    const DEFAULT_MODE = 'login';
    const CLOSE_DELAY_MS = 900;

    const modeConfig = {
        login: {
            success: 'Login successful. Your Finex dashboard is ready.'
        },
        signup: {
            success: 'Account created successfully. Welcome to Finex Bank.'
        }
    };

    const modals = Array.from(document.querySelectorAll(SELECTORS.modal));

    if (modals.length === 0) {
        return;
    }

    const openButtons = Array.from(document.querySelectorAll(SELECTORS.openButton));
    const closeButtons = Array.from(document.querySelectorAll(SELECTORS.closeButton));
    const switchButtons = Array.from(document.querySelectorAll(SELECTORS.switchButton));
    const forms = Array.from(document.querySelectorAll(SELECTORS.form));

    const modalByMode = modals.reduce((map, modal) => {
        const mode = modal.getAttribute('data-auth-modal');

        if (mode) {
            map.set(mode, modal);
        }

        return map;
    }, new Map());

    const resolveModeName = (mode) => (modeConfig[mode] ? mode : DEFAULT_MODE);

    const focusFirstInput = (modal) => {
        const firstField = modal?.querySelector(SELECTORS.firstInput);

        if (firstField) {
            firstField.focus();
        }
    };

    const closeAllModals = () => {
        modals.forEach((modal) => {
            modal.hidden = true;
        });

        document.body.classList.remove('auth-open');
    };

    const openModal = (mode) => {
        const targetMode = resolveModeName(mode);
        const targetModal = modalByMode.get(targetMode);

        if (!targetModal) {
            return;
        }

        closeAllModals();
        targetModal.hidden = false;
        document.body.classList.add('auth-open');
        focusFirstInput(targetModal);
    };

    const showSuccessMessage = (form, mode) => {
    const modal = form.closest(SELECTORS.modal);
    const feedbackElement = modal?.querySelector(SELECTORS.feedback);

    if (feedbackElement) {
        feedbackElement.textContent = modeConfig[resolveModeName(mode)].success;
    }
    };

    const handleFormSubmit = (event) => {
        event.preventDefault();

        const form = event.currentTarget;
        if (!(form instanceof HTMLFormElement)) {
            return;
        }

        if (!form.reportValidity()) {
            return;
        }

        const mode = form.getAttribute('data-auth-form');
        showSuccessMessage(form, mode);
        form.reset();

        window.setTimeout(closeAllModals, CLOSE_DELAY_MS);
    };

    const hasOpenedModal = () => modals.some((modal) => !modal.hidden);

    openButtons.forEach((button) => {
        button.addEventListener('click', () => {
            openModal(button.getAttribute('data-auth-mode'));
        });
    });

    closeButtons.forEach((button) => {
        button.addEventListener('click', closeAllModals);
    });

    switchButtons.forEach((button) => {
        button.addEventListener('click', () => {
            openModal(button.getAttribute('data-auth-switch'));
        });
    });

    forms.forEach((form) => {
        form.addEventListener('submit', handleFormSubmit);
    });

    document.addEventListener('keydown', (event) => {
        if (!hasOpenedModal()) {
            return;
        }

        if (event.key === 'Escape') {
            closeAllModals();
        }
    });
})();
